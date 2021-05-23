
/*
 * File description:
 *   Header file for function implementing bitonic sorting serially in C.
 */

#ifndef NAIVE_BITONIC_SORT_SERIAL_H
#define NAIVE_BITONIC_SORT_SERIAL_H 

#include "naive_bitonic_sort_opencl.h"

// Define message printed out to user signaling start of serial bitonic sort
#if (ARRAY_TYPE == CHAR)
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting chars"\
                                          " with serial bitonic sort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting chars"\
                                          " with serial bitonic sort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == INT) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting ints"\
                                          " with serial bitonic sort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting ints"\
                                          " with serial bitonic sort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == LONG) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting longs"\
                                          " with serial bitonic sort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting longs"\
                                          " with serial bitonic sort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == FLOAT) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting floats"\
                                          " with serial bitonic sort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting floats"\
                                          " with serial bitonic sort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == DOUBLE) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting doubles"\
                                          " with serial bitonic sort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_SORT_SERIAL_START ">>> Starting sorting doubles"\
                                          " with serial bitonic sort on CPU, sort descending...\n"
  #endif
#endif

/*
 * Serial implementation of bitonic sort in C.
 * - "input_array" is the padded array to be sorted using bitonic sort.
 * - "sorting_direction" indicates whether or not to sort array in ascending or descending order;
 *   refer to SORTING_DIRECTION macro within "naive_bitonic_sort_opencl.h" for more info.
 */
void serial_bitonic_sort(struct Array_With_Length_Padded* input_array, const unsigned int sorting_direction);

#endif // NAIVE_BITONIC_SORT_SERIAL_H

