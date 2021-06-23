
// =================================================================================================
// File description:
// Header file for custom OpenCL bitonic sorting host functions that allocate device memory for
// the sorting step function and then perform the actual sorting; first function in this file
// is responsible for setting up parameters for ONE KERNEL ONLY, and then the second executes
// that kernel repeatedly.
//
// Implementation of Kernel and sorting method inspired by the following websites:
// - https://en.wikipedia.org/wiki/Bitonic_sorter
// - http://www.bealto.com/gpu-sorting_parallel-bitonic-1.html
// - https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/BitonicSort.h
// - https://gist.github.com/mre/1392067
//
// Original file information:
// Institution.... SURFsara <www.surfsara.nl>
// Original Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// "Remixing" programmer.. Ted Li
// Changed at..... 2020-05-15
// License........ MIT license
// 
// =================================================================================================

#ifndef NAIVE_BITONIC_SORT_OPENCL_H
#define NAIVE_BITONIC_SORT_OPENCL_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

/*
 * Flag macro literals indicating whether sorting
 * an array in ascending or desending order
 */
#define ASCENDING_SORT 0
#define DESCENDING_SORT 1
// Flag macro indicating direction of sort for algorithm
#define SORTING_DIRECTION DESCENDING_SORT

/*
 * Custom definitions of permitted macro values
 * for ARRAY_TYPE
 */
#define CHAR 0
#define INT 1
#define LONG 2
#define FLOAT 3
#define DOUBLE 4
/* 
 * Data type of each element in the array being sorted
 * Permitted macro values are the following:
 * - CHAR
 * - INT
 * - LONG
 * - FLOAT
 * - DOUBLE 
 */
#define ARRAY_TYPE DOUBLE
/*
 * Declared type of each array within OpenCL
 * and host program files. Also customize message
 * to user about what's being sorted based on
 * data type and sorting direction.
 */
#if(ARRAY_TYPE == CHAR)
  #define ARRAY_TYPE_DECLARED char
  #define COMPILER_ARRAY_TYPE_OPTION "-DARRAY_TYPE=char"
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of chars"\
                                            " with %d work-items per workgroup, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of chars"\
                                            " with %d work-items per workgroup, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == INT)
  #define ARRAY_TYPE_DECLARED int
  #define COMPILER_ARRAY_TYPE_OPTION "-DARRAY_TYPE=int"
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of ints"\
                                             " with %d work-items per workgroup, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of ints"\
                                             " with %d work-items per workgroup, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == LONG)
  #define ARRAY_TYPE_DECLARED long
  #define COMPILER_ARRAY_TYPE_OPTION "-DARRAY_TYPE=long"
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of longs"\
                                              " with %d work-items per workgroup, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of longs"\
                                              " with %d work-items per workgroup, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == FLOAT)
  #define ARRAY_TYPE_DECLARED float
  #define COMPILER_ARRAY_TYPE_OPTION "-DARRAY_TYPE=float"
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of floats"\
                                                " with %d work-items per workgroup, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of floats"\
                                                " with %d work-items per workgroup, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == DOUBLE)
  #define ARRAY_TYPE_DECLARED double
  #define COMPILER_ARRAY_TYPE_OPTION "-DARRAY_TYPE=double"
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of doubles"\
                                                " with %d work-items per workgroup, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_OPENCL_START ">>> Starting OpenCL parallelized bitonic sorting of doubles"\
                                                " with %d work-items per workgroup, sort descending...\n"
  #endif
#endif

/*
 * Threadblock sizes; 64 - 128 is what Intel recommends for most algorithms I believe
 * DO NOT USE THREAD BLOCKS LARGER THAN 256; it WILL BREAK the algorithm.
 */
#define NUM_THREADS_IN_BLOCK 256
// Amount of offset of starting location of buffer contents in device memory
#define CL_BUFFER_OFFSET 0
/*
 * Number of dimensions of problem being solved
 *   In this case it's 1 b/c we're sorting a 1D array.
 */
#define OPERAND_DIMS 1

// File in which program containing OpenCL kernels is stored
#define PROGRAM_FILE "bitonic_program.cl"
// Name of kernel function in OpenCL program file
#define KERNEL_FUNC_NAME "naive_bitonic_sort_merge_step"

/*
 * Flag variable literals indicating whether the
 * array padding for "Array_With_Length_Padded" occurs
 * at beginning or the end of the array.
 */
#define PAD_ARRAY_AT_BEGINNING 1
#define PAD_ARRAY_AT_END 0
/*
 * A struct in C where the following four
 * fields are grouped together:
 *  - pointer indicating beginning of array
 *  - actual length of array as "array_len_actual"
 *  - "padded_2n_length" indicating the total number
 *    of elements in the array after the array 
 *    has been padded with extra elements (with
 *    each element of the highest value possible)
 *    so that the array length is a power of 2.
 *  - flag variable indicating whether or not
 *    the padding of the array occurs at the
 *    beginning or the end of the array
 */
struct Array_With_Length_Padded {
     ARRAY_TYPE_DECLARED* contents;
     unsigned int array_len_actual;
     unsigned int padded_2n_length;
     unsigned int padding_location_indicator;
};


/* 
 * Load array to be sorted using bitonic sort into OpenCL device's memory;
 * the data will processed by the kernel later on the OpenCL device.
 * The array HAS TO BE at least of length 1.
 * Parameter details:
 *   - context --- the OpenCL execution context for which the load the array
 *   - queue --- the OpenCL command queue created from the aforementioned "context"
 *                in which to enqueue a write command to load the array into OpenCL
 *                device memory.
 *   - input_array --- A pointer to a struct containing the array to be loaded into
 *                     OpenCL device's memory.
 *   - buffer_in --- a pointer to a memory handle where the handle corresponds
 *                      to the array copied over from main memory into the OpenCL
 *                      device's memory.
 */
void load_array_bitonic_sort(cl_context *context, cl_command_queue* queue, 
                               struct Array_With_Length_Padded* input_array, cl_mem* buffer_in);

/* 
 * Parameter details:
 * - queue --- the OpenCL command queue in which to enqueue commands for
 *              sorting an array using bitonic sort.
 * - cl_program* program --- MUST point to a program in memory which has an in-place
 *                           bitonic sort kernel function of signature:
 *                           __kernel void naive_bitonic_sort_merge_step(__global ARRAY_TYPE* input_array,
 *                                                                        const unsigned int compare_distance,
 *                                                                            const unsigned int partition_size,
 *                                                                              const unsigned int sort_direction)
 * - cl_kernel* kernel --- must point to a kernel function in memory whose function signature
 *                         is the "naive_bitonic_sort_merge_step" signature specified above.
 * - input_array --- a struct containing a pointer to the array to be sorted and a field
 *                          storing the array's length; the array is to be sorted using
 *                          bitonic sort.
 * - buffer_in --- a pointer to a memory handle corresponding to a copy of the array to be sorted
 *                 within the OpenCL device's memory.
 * - sorting_direction --- "DESCENDING_SORT" (i.e. non-zero value) indicates to sort descending;
 *                          "ASCENDING_SORT" (i.e. zero) indicates to sort ascending.
 *
 * Custom implementation of bitonic sorting using OpenCL;
 * each sorting step is performed within device memory
 * The array being sorted HAS TO BE at least of length 1.
 */
void opencl_bitonic_sort(cl_command_queue *queue, cl_program *program,
                           cl_kernel* kernel, struct Array_With_Length_Padded* input_array,
                                       cl_mem* buffer_in, const unsigned int sorting_direction);

#endif // NAIVE_BITONIC_SORT_OPENCL_H
// =================================================================================================

