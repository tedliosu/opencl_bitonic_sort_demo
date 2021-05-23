
/* 
 * File description: Serial version of bitonic sorting.
 *   Implementation based on https://en.wikipedia.org/wiki/Bitonic_sorter#Example_code
 */

#include "naive_bitonic_sort_serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

// Merges pairs of bitonic sequences in an array into bigger bitonic sequences
static inline void serial_bitonic_sort_merge_step(ARRAY_TYPE_DECLARED* input_array,
                                                      const unsigned int array_length,
                                                       const unsigned int compare_distance,
                                                            const unsigned int partition_size,
                                                                const unsigned int sort_direction) {
   // Null pointer not allowed
   assert(input_array != NULL);
   // Array-related and sorting parameters all have to be greater than zero.
   assert(array_length > 0);
   assert(compare_distance > 0);
   assert (partition_size > 0);
   // Make sure sort_direction is of valid value
   assert((sort_direction == ASCENDING_SORT) || (sort_direction == DESCENDING_SORT));

   /*
    * Constant flag variable representing the ascending part of a bitonic sequence if sorting
    * elements in ascending order, the descending part if sorting in descending order.
    */
   const unsigned int monotonic_part_indicator = 0;
   
   // Iterate over the array and swap numbers as necessary.
   for (unsigned int array_index = 0; array_index < array_length; ++array_index) {

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
                ARRAY_TYPE_DECLARED temp_var = input_array[array_index];
                input_array[array_index] = input_array[compare_distance_rotated_index];
                input_array[compare_distance_rotated_index] = temp_var;
           }
       }

   }

}


void serial_bitonic_sort(struct Array_With_Length_Padded* input_array, const unsigned int sorting_direction) {

    // Parameter cannot be NULL
    assert(input_array != NULL);
    assert(input_array->contents != NULL);
    // Array length has to be non-zero
    assert(input_array->array_len_actual > 0);
    assert(input_array->padded_2n_length > 0);
    // Check that padding location indicator is of valid value
    assert((input_array->padding_location_indicator == PAD_ARRAY_AT_BEGINNING) ||
                         (input_array->padding_location_indicator == PAD_ARRAY_AT_END));
    // Make sure sort_direction is of valid value
    assert((sorting_direction == ASCENDING_SORT) || (sorting_direction == DESCENDING_SORT));

    /* 
     * Get padded length of array for bitonic sort, since iterative bitonic
     * sort requires array lengths to be a power of 2.
     */
    const unsigned int padded_array_len = input_array->padded_2n_length;
    // Get pointer to beginning of array to be sorted
    ARRAY_TYPE_DECLARED* array_begin = input_array->contents;

    // Notify user serial bitonic sorting starts now
    printf(NOTIFY_USER_SORT_SERIAL_START);

    /* 
     * Iterate over all different partition sizes for array, where each partition is half of the
     * subarray of each of the bitonic sequences being created during each iteration.
     */
    for (unsigned int partition_size = 2; partition_size <= padded_array_len; partition_size *= 2) {
        /*
         * Iterate over all different compare distances, where each compare distance is how far
         * apart the numbers being compared are for constructing the bitonic sequences.
         */
        for (unsigned int compare_distance = partition_size / 2; compare_distance > 0; compare_distance /= 2) {
            /*
             * For each iteration, rearrange numbers in the array in main memory to create bitonic sequences of
             * length = twice the partition size using all possible different compare distances, where
             * each compare distance is a power of 2.
             */
            serial_bitonic_sort_merge_step(array_begin, padded_array_len, compare_distance,
                                                           partition_size, sorting_direction);

        }
    }

}

