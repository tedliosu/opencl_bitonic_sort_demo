
// =================================================================================================
// File description:
// Contains implementations of host functions for bitonic sorting; 1st function in this file is to
// set up parameters for executing ONE KERNEL, and the 2nd function in this file repeatedly executes
// the kernel to sort numbers bitonically.
//
// Original file information:
// Institution.... SURFsara <www.surfsara.nl>
// Original Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// "Remixing" programmer.. Ted Li
// Changed at..... 2021-05-15
// License........ MIT license
// 
// =================================================================================================

// Libraries used by the functions in this file with custom headers
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "naive_bitonic_sort_opencl.h"

// =================================================================================================

void load_arrays_bitonic_sort(cl_context *context, cl_command_queue* queue,
                                            struct List_Of_Arrays array_list,
                                        struct Cl_Mem_Operands_List cl_operands) {
    
    // No null pointers allowed
    assert(context != NULL);
    assert(queue != NULL);
    assert(array_list.array_in != NULL);
    assert(array_list.array_in->contents != NULL);
    assert(cl_operands.buffer_in != NULL);
    // Array length HAS to be at least 1
    assert(array_list.array_in->array_len_actual >= 1);
    assert(array_list.array_in->padded_2n_length >= 1);
    // Check that padding location indicator is of valid value
    assert((array_list.array_in->padding_location_indicator == PAD_ARRAY_AT_BEGINNING) ||
                      (array_list.array_in->padding_location_indicator == PAD_ARRAY_AT_END));

    cl_int func_error_code;

    // Create buffer to be filled on OpenCL device based on execution environment.
    *(cl_operands.buffer_in) = clCreateBuffer(*context, CL_MEM_READ_WRITE, 
                                                 (array_list.array_in->padded_2n_length)*
                                                                  sizeof(ARRAY_TYPE_DECLARED),
                                                                             NULL, &func_error_code);

    /*
     * Copy array to the buffer created on the OpenCL device
     * Note: if I try to do sizeof([insert variable here]), most of the contents of my arrays get eaten by an 
     *    unknown force in my computer, not sure why but I thought that is something interesting to document
     */
    func_error_code = clEnqueueWriteBuffer(*queue, *(cl_operands.buffer_in), CL_BLOCKING,
                                              CL_BUFFER_OFFSET, (array_list.array_in->padded_2n_length) *
                                                                                 sizeof(ARRAY_TYPE_DECLARED),
                                                                  array_list.array_in->contents, 0, NULL, NULL);
}

void opencl_bitonic_sort(cl_command_queue *queue, cl_program *program,
                          cl_kernel* kernel, struct List_Of_Arrays array_list,
                                       struct Cl_Mem_Operands_List cl_operands,
                                             const unsigned int sorting_direction) {
    // No null pointers allowed
    assert(program != NULL);
    assert(queue != NULL);
    assert(kernel != NULL);
    assert(array_list.array_in != NULL);
    assert(cl_operands.buffer_in != NULL);
    assert(array_list.array_in->contents != NULL);
    // Array length HAS to be at least 1
    assert(array_list.array_in->array_len_actual >= 1);
    assert(array_list.array_in->padded_2n_length >= 1);
    // Check that padding location indicator is of valid value
    assert((array_list.array_in->padding_location_indicator == PAD_ARRAY_AT_BEGINNING) ||
                      (array_list.array_in->padding_location_indicator == PAD_ARRAY_AT_END));
    // Make sure sort_direction is of valid value
    assert((sorting_direction == ASCENDING_SORT) || (sorting_direction == DESCENDING_SORT));

    // The last event to be performed in the command queue on the OpenCL device
    cl_event event;

    /*
     * Generate the kernel runtime and set 1st argument of kernel to address of loaded buffer
     * and last argument to indicate direction of sort.
     */
    *kernel = clCreateKernel(*program, KERNEL_FUNC_NAME, NULL);
    clSetKernelArg(*kernel, 0, sizeof(*(cl_operands.buffer_in)), (void*)cl_operands.buffer_in);
    clSetKernelArg(*kernel, 3, sizeof(sorting_direction), (void*)&sorting_direction);
    
    /* 
     * Specify size of each thread block and size of array to be sorted 
     * for each time the kernel is called.
     */
    const size_t local[OPERAND_DIMS] = { NUM_THREADS_IN_BLOCK };
    const size_t global[OPERAND_DIMS] = { array_list.array_in->padded_2n_length };

    // Notify user sorting starts now
    printf(NOTIFY_USER_SORT_OPENCL_START, NUM_THREADS_IN_BLOCK);

    /* 
     * Iterate over all different partition sizes for array, where each partition is half of the
     * subarray of each of the bitonic sequences being created during each iteration.
     */
    for (unsigned int partition_size = 2; partition_size <= array_list.array_in->padded_2n_length;
                                                                                 partition_size *= 2) {
          /*
           * Iterate over all different compare distances, where each compare distance is how far
           * apart the numbers being compared are for constructing the bitonic sequences.
           */     
        for ( unsigned int compare_distance = partition_size / 2; compare_distance > 0; compare_distance /= 2) {
             /*
              * For each iteration, rearrange numbers in the array on device memory to create bitonic sequences of
              * length = twice the partition size using all possible different compare distances, where
              * each compare distance is a power of 2.
              */
             clSetKernelArg(*kernel, 1, sizeof(compare_distance), (void*)&compare_distance);
             clSetKernelArg(*kernel, 2, sizeof(partition_size), (void*)&partition_size);
             clEnqueueNDRangeKernel(*queue, *kernel, OPERAND_DIMS, NULL, global, local, 0, NULL, &event);
        }
    }

    // Wait for all sorting to be finished
    clWaitForEvents(1, &event);

} 

// =================================================================================================
