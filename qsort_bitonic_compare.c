
// =================================================================================================
// Project: 
// Exploring bitonic sorting in OpenCL.
//
// Original file information:
// Institution.... SURFsara <www.surfsara.nl>
// Original Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// "Remixing" programmer.. Ted Li
// Changed at..... 2021-05-15
// License........ MIT license
// 
// =================================================================================================

// Libraries used by this program with custom headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <assert.h>
#include "array_utilities.h"
#include "naive_bitonic_sort_opencl.h"
#include "naive_bitonic_sort_serial.h"
#include "qsort_bitonic_compare.h"

// =================================================================================================


/* 
 * Given a specified file location containing an OpenCL program
 * reads the entire contents of the file into memory and returns
 * a pointer marking the start of the contents in memory.
 */
char* get_opencl_program_code(char* opencl_program_file_location) {
   
      // No null pointers allowed
      assert(opencl_program_file_location != NULL);

      char *source_code_content = NULL;
      const char *file_read_mode = "r";
      const char *file_open_error_msg = "Error opening OpenCL program file: ";
      const char *file_io_error_msg = "Error reading OpenCL program file: ";
      FILE *opencl_prog_file = fopen(opencl_program_file_location, file_read_mode);

      if (opencl_prog_file == NULL) {
            perror(file_open_error_msg);
      } else {
            size_t init_buffer_size = 0;
            ssize_t num_bytes_read = getdelim(&source_code_content, &init_buffer_size,
                                                      TEXT_FILE_DELIM, opencl_prog_file);
            if (num_bytes_read < 0) {
                perror(file_io_error_msg);
            }
      }

      return source_code_content;

}

/*
 * Setup procedure for executing OpenCL programs.  The procedure involves
 * creating an execution context to be used by the OpenCL-programmed device
 * (in this case the GPU or CPU), setting up the queue which is used to store
 * the kernels to be executed by the device within the execution context, and
 * then dynamically compiling the program containing the kernels which are to be
 * executed once the kernels (or even multiple copies of each kernel) gets added
 * to the queue.
 */
void configure_opencl_env(cl_context *context, cl_command_queue* queue,
                                                     cl_program *program) {
    
    // No null pointers allowed
    assert(context != NULL);
    assert(queue != NULL);
    assert(program != NULL);

    cl_device_id device;
    char deviceName[MAX_LEN];
    cl_queue_properties queue_properties[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
    cl_platform_id *platforms = malloc(sizeof(cl_platform_id) * NUM_CL_PLATFORMS);
    
    clGetPlatformIDs(NUM_CL_PLATFORMS, platforms, NULL);
    clGetDeviceIDs(platforms[DESIRED_PLATFORM_INDEX], CL_DEVICE_TYPE_DEFAULT, NUM_CL_DEVICES, &device, NULL);
    *context = clCreateContext(NULL, NUM_CL_DEVICES, &device, NULL, NULL, NULL);
    *queue = clCreateCommandQueueWithProperties(*context, device, queue_properties, NULL);

    // read OpenCL program file into string
    const char* opencl_program_string = get_opencl_program_code(PROGRAM_FILE);

    clGetDeviceInfo(device, CL_DEVICE_NAME, MAX_LEN, deviceName, NULL);


    if (opencl_program_string != NULL) {

        // Compile the opencl_program
        *program = clCreateProgramWithSource(*context, OPENCL_PROGS, &opencl_program_string, NULL, NULL);
        clBuildProgram(*program, 0, NULL, COMPILER_ARRAY_TYPE_OPTION, NULL, NULL);

        // Get info generated by compiler and output any compiler-generated messages to user
        size_t logSize;
        clGetProgramBuildInfo(*program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
        char* messages = (char*)malloc((1+logSize)*sizeof(char));
        clGetProgramBuildInfo(*program, device, CL_PROGRAM_BUILD_LOG, logSize, messages, NULL);
        messages[logSize] = TEXT_FILE_DELIM;
        printf(">>> OpenCL program compiler result message: - %s\n\n", messages); 
        free(messages);
    
    }

    // Platforms already acquired; free malloc'ed memory
    free(platforms);

}


// Element comparision function for qsort
int compare_elements_qsort(const void *first_arg, const void *second_arg) {

    // No null pointers allowed
    assert(first_arg != NULL);
    assert(second_arg != NULL);

    // Cast to appropriate data types as determined by "control" macros
    ARRAY_TYPE_DECLARED first_element = *(const ARRAY_TYPE_DECLARED *) first_arg;
    ARRAY_TYPE_DECLARED second_element = *(const ARRAY_TYPE_DECLARED *) second_arg;
    // Compare result as defined in the C standard library documentation for qsort
    int compare_result;

    // Compare-and-subtract-trick taken from https://en.wikipedia.org/wiki/Qsort
    if (SORTING_DIRECTION) {
      compare_result = (first_element < second_element) - (first_element > second_element);
    } else {
      compare_result = (first_element > second_element) - (first_element < second_element);
    }

    return compare_result;

}


// Testing bitonic sorting using a custom OpenCL opencl_program.
int main(int argc, char* argv[]) {
    
    // All variable declarations
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem buffer_in;
    struct timespec current_time;
    double sort_start_time, sort_end_time;
    struct Array_With_Length_Padded* sample_array = get_rand_padded_array(ARRAY_LEN);
    struct Array_With_Length_Padded* sample_array_cp = deep_cp_padded_array(sample_array);
    struct Array_With_Length_Padded* sample_array_2nd_cp = deep_cp_padded_array(sample_array);
    struct List_Of_Arrays array_list = { sample_array };
    struct Cl_Mem_Operands_List cl_mem_ops = { &buffer_in };
    
    configure_opencl_env(&context, &queue, &program);

    // Get time of when parallel bitonic sort algorithm starts executing
    timespec_get(&current_time, TIME_UTC);
    sort_start_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    load_arrays_bitonic_sort(&context, &queue, array_list, cl_mem_ops);

    opencl_bitonic_sort(&queue, &program, &kernel, array_list, cl_mem_ops, SORTING_DIRECTION);

    // Copy the sorted array back to the CPU memory
    clEnqueueReadBuffer(queue, *(cl_mem_ops.buffer_in), CL_BLOCKING, CL_BUFFER_OFFSET,
                                 sample_array->padded_2n_length*sizeof(ARRAY_TYPE_DECLARED),
                                                         sample_array->contents, 0, NULL, NULL);

    // Get time of when parallel bitonic sort finishes executing
    timespec_get(&current_time, TIME_UTC);
    sort_end_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    // Report to user time spent on sorting using parallelized bitonic sort in OpenCL
    printf(BITONIC_PARALLEL_SORT_MESSAGE, sample_array->array_len_actual, sort_end_time - sort_start_time);
 
    // Get time of when serial bitonic sort algorithm starts executing
    timespec_get(&current_time, TIME_UTC);
    sort_start_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    serial_bitonic_sort(sample_array_cp, SORTING_DIRECTION);

    // Get time of when serial bitonic sort finishes executing
    timespec_get(&current_time, TIME_UTC);
    sort_end_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    // Report to user time spent on sorting using serial bitonic sort on CPU
    printf(BITONIC_SERIAL_SORT_MESSAGE, sample_array_cp->array_len_actual, sort_end_time - sort_start_time);

    // Signal to user start of Qsort
    printf(NOTIFY_USER_QSORT_START);

    // Get time of when qsort starts executing
    timespec_get(&current_time, TIME_UTC);
    sort_start_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    qsort(sample_array_2nd_cp->contents, sample_array_2nd_cp->padded_2n_length,
                    sizeof(*(sample_array_2nd_cp->contents)), compare_elements_qsort);

    // Get time of when qsort finishes executing
    timespec_get(&current_time, TIME_UTC);
    sort_end_time = (double) current_time.tv_sec + ((double) current_time.tv_nsec) / NANOSECS_IN_SEC;

    // Report to user time spent on sorting using qsort
    printf(QSORT_MESSAGE, sample_array_2nd_cp->array_len_actual, sort_end_time - sort_start_time);

    /*
     * if sorting direction was descending, have padding location indicator be adjusted to 
     * indicate padding at beginning of the array.
     */
    if (SORTING_DIRECTION) {
       sample_array->padding_location_indicator = PAD_ARRAY_AT_BEGINNING;
       sample_array_cp->padding_location_indicator = PAD_ARRAY_AT_BEGINNING;
       sample_array_2nd_cp->padding_location_indicator = PAD_ARRAY_AT_BEGINNING;
    }

    // Assert that all sorting was done correctly
    assert_padded_arrays_equality(sample_array, sample_array_2nd_cp);
    assert_padded_arrays_equality(sample_array_cp, sample_array_2nd_cp);

    /* 
     * Give back to the system both OpenCL device memory space and main
     * system memory space used during program execution.
     */
    clReleaseMemObject(buffer_in);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseKernel(kernel);

    // Free the host memory objects
    free(sample_array->contents);
    free(sample_array);
    free(sample_array_cp->contents);
    free(sample_array_cp);
    free(sample_array_2nd_cp->contents);
    free(sample_array_2nd_cp);

    return EXIT_SUCCESS;
}

// =================================================================================================
