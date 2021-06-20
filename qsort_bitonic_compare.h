
// =================================================================================================
// File Description: 
// Bitonic sorting in OpenCL main header file 
//
// Original file information:
// Institution.... SURFsara <www.surfsara.nl>
// Original Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// Refactoring programmer.. Ted Li
// Changed at..... 2021-04-02
// License........ MIT license
// 
// =================================================================================================

#ifndef QSORT_BITONIC_COMPARE_H
#define QSORT_BITONIC_COMPARE_H 

#include "naive_bitonic_sort_opencl.h"

/*
 * Size of array to be sorted via bitonic sort; 
 * The following number has to be greater than 0!
 */
#define ARRAY_LEN 100000000

// Number of OpenCL platforms on host machine
#define NUM_CL_PLATFORMS 2
/*
 * Index of desired OpenCL platform (e.g. Portable Computing
 *    Language, AMD Accelerated Parallel Processing) in
 *    list of platforms returned.
 */
#define DESIRED_PLATFORM_INDEX 1
// Number of OpenCL devices per OpenCL platform
#define NUM_CL_DEVICES 1
// Number of OpenCL programs to be loaded
#define OPENCL_PROGS 1
// OpenCL device name max length
#define MAX_LEN 1024
// Delimiter for reading text files
#define TEXT_FILE_DELIM '\0'

// Define message printed out to user signaling start of qsort (from standard C libraries).
#if (ARRAY_TYPE == CHAR)
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of chars"\
                                          " with qsort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of chars"\
                                          " with qsort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == INT) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of ints"\
                                          " with qsort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of ints"\
                                          " with qsort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == LONG) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of longs"\
                                          " with qsort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of longs"\
                                          " with qsort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == FLOAT) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of floats"\
                                          " with qsort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of floats"\
                                          " with qsort on CPU, sort descending...\n"
  #endif
#elif (ARRAY_TYPE == DOUBLE) 
  #if(SORTING_DIRECTION == ASCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of doubles"\
                                          " with qsort on CPU, sort ascending...\n"
  #elif(SORTING_DIRECTION == DESCENDING_SORT)
    #define NOTIFY_USER_QSORT_START ">>> Starting sorting of doubles"\
                                          " with qsort on CPU, sort descending...\n"
  #endif
#endif

// Number of nanoseconds in a second
#define NANOSECS_IN_SEC 1000000000.0

// Messages to user informing time took to sort and how many numbers were sorted
#define BITONIC_PARALLEL_SORT_MESSAGE "Parallelized bitonic sort of %d element(s)"\
                                            " on OpenCL device took %lf seconds\n\n"
#define BITONIC_SERIAL_SORT_MESSAGE "Serial bitonic sort on CPU of %d element(s) in main memory took %lf seconds\n\n"
#define QSORT_MESSAGE "Qsort on CPU of %d element(s) in main memory took %lf seconds\n\n"

// Messages informing user what kind of sorting result verification program is performing
#define BITONIC_PARALLEL_SORT_VERIFY_MSG ">>> Verifying correctness of parallelized bitonic sort on OpenCL device...\n"
#define BITONIC_SERIAL_SORT_VERIFY_MSG ">>> Verifying correctness of serial bitonic sort in main memory...\n"

#endif // QSORT_BITONIC_COMPARE_H
// =================================================================================================

