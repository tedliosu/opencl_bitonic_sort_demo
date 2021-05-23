

// Enable 64-bit floating point compute
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

/*
 * Bitonic sorting merge kernel operating on global memory in OpenCL.
 *  Implementation of Kernel and sorting method inspired by the following websites:
 *   - https://en.wikipedia.org/wiki/Bitonic_sorter
 *   - http://www.bealto.com/gpu-sorting_parallel-bitonic-1.html
 *   - https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/BitonicSort.h
 *   - https://gist.github.com/mre/1392067
 */
__kernel void naive_bitonic_sort_merge_step(__global ARRAY_TYPE* input_array, const unsigned int compare_distance,
                                                                                  const unsigned int partition_size,
                                                                                   const unsigned int sort_direction)
{
   /* 
    * Value representing the first work-item dimension in the OpenCL programming model, as arrays
    * are one-dimensional data structures and therefore we are only concerned with that first
    * dimension.
    */
   const unsigned int first_dimension_num = 0;
   /*
    * Constant flag variable representing the ascending part of a bitonic sequence if sorting
    * elements in ascending order, the descending part if sorting in descending order.
    */
   const unsigned int monotonic_part_indicator = 0;
   
   /* The current index of the array this kernel is performing a bitonic sorting step on. */
   unsigned int array_index = get_global_id(first_dimension_num);
   
   /*
    * Rotate the current array index using XOR to the index of the
    * other number to compare to as specified by the bitonic sorting
    * network.
    */
   unsigned int compare_distance_rotated_index = compare_distance ^ array_index;

   /*
    * Only make comparisons and swap as necessary if compare_distance_rotated_index
    * is larger to avoid double comparisons; the contents of this "if" control structure
    * is responsible for creating the monotonic halves of a bitonic sequence by using
    * a bitmask (i.e. "bitonic_sequence_part_indicator") to determine which numbers
    * should be swapped.  The bitmask partitions the array into ascending and descending
    * halves. If the bitmask indicates to sort ascending, then sort ascending by
    * swapping numbers which are in descending order, and vice versa for bitmasks
    * indicating to sort descending.
    */
   if (compare_distance_rotated_index > array_index) {
      /*
       * The next three lines of code determine whether or not to swap
       * the number at array_index with the number at compare_distance_rotated_index
       * within the array by identifying which part of the bitonic sequence
       * each number is supposed to be during the next sorting step (i.e. merging
       * the ascending and descending halves of a bitonic sequence into a larger
       * monotonic sequence). This is done through bit-masking the array_index by
       * the partition_size and then making the appropriate swaps based on where
       * each number falls under the bit-masked result.
       */
       unsigned int bitonic_sequence_part_indicator = array_index & partition_size;
       // Negate sequence part indicator if sorting elements in descending order.
       if (sort_direction) {
               bitonic_sequence_part_indicator = ! bitonic_sequence_part_indicator;
       }
       /*
        * Once the partition_size equals the length of the array being sorted,
        * then half of the conditional statement as written below (either the half
        * before the "or" operator or after the "or" operator depending on whether
        * the array is to be sorted in ascending or descending order respectively)
        * no longer applies (i.e. gets "cancelled out") to the bitonic sorting
        * procedure, and so the sorting procedure concludes with using ONLY the other
        * half of the conditional statement to merge together the ascending and
        * descending halves of the *final* bitonic sequence in the array into a
        * sorted array.
        */
       bool swap = (bitonic_sequence_part_indicator == monotonic_part_indicator &&
                         input_array[array_index] > input_array[compare_distance_rotated_index]) ||
                     (bitonic_sequence_part_indicator != monotonic_part_indicator &&
                         input_array[array_index] <= input_array[compare_distance_rotated_index]);

       // Swap numbers as necessary
       if (swap) {
           ARRAY_TYPE temp_var = input_array[array_index];
           input_array[array_index] = input_array[compare_distance_rotated_index];
           input_array[compare_distance_rotated_index] = temp_var;
      }
   }
   
}

