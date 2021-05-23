/* 
 * Author - Ted Li
 *
 * This file defines a function which generates a
 *    "padded array" of random numbers/characters.
 */

#ifndef ARRAY_UTILITIES_H
#define ARRAY_UTILITIES_H

#include "naive_bitonic_sort_opencl.h"

/*
 * Format string used to print out values to user;
 * depends on ARRAY_TYPE from "naive_bitonic_sort_opencl.h"
 */
#if(ARRAY_TYPE == CHAR)
    #define DISPLAY_FORMAT_STR "%c "
#elif (ARRAY_TYPE == INT)
    #define DISPLAY_FORMAT_STR "%d "
#elif (ARRAY_TYPE == LONG)
    #define DISPLAY_FORMAT_STR "%ld "
#elif (ARRAY_TYPE == FLOAT)
    #define DISPLAY_FORMAT_STR "%f "
#elif (ARRAY_TYPE == DOUBLE)
    #define DISPLAY_FORMAT_STR "%.12lf "
#endif

/* 
 * The pointer returned by this function points
 *   to an "Array_With_Length_Padded" struct
 *   where the "contents" field points to an
 *   array of numbers/characters (each element
 *   type is determined by the "ARRAY_TYPE" macro);
 *   The array is of length 2^ceil(log2(array_len)),
 *   and that length is stored in the "padded_2n_length"
 *   field in the struct. The value of this function's
 *   parameter is stored within "array_len_actual"
 *   in the struct. And for the first "array_len_actual"
 *   elements in the array, for integer types
 *   (e.g. int, long char) each value in the
 *   array is randomly generated and may range
 *   from the minimum permitted to the maximum
 *   permitted value (e.g. INT_MIN to INT_MAX)
 *   of each type. As for floating point types
 *   (e.g. float, double), each value ranges
 *   from -1.0 to 1.0. For any additional elements
 *   up to the "padded_2n_length"-th element, each
 *   element is of the element's maximum value
 *   type (e.g. INT_MAX, DBL_MAX). Finally, 
 *   as this function always adds the additional
 *   elements at the end of the aforementioned array,
 *   "padding_location_indicator" will be set to the
 *   value of the macro "PAD_ARRAY_AT_END".
 */
struct Array_With_Length_Padded *get_rand_padded_array(const unsigned int array_len);

/* 
 * Returns a pointer to a deep copy of the parameter;
 *   ONLY works with "Array_With_Length_Padded" types.
 */
struct Array_With_Length_Padded *deep_cp_padded_array(struct Array_With_Length_Padded* padded_array);

/*
 * Prints out the contents of the array stored
 *   within an "Array_With_Length_Padded"
 */
void print_array(struct Array_With_Length_Padded* array);

/*
 * Message informing user that two "Array_With_Length_Padded"s are equal to each other
 */
#define ASSERTION_PASSED_INFORM_USER "Congratulations, both of your padded arrays are identical in content!\n"
/*
 * Asserts that two different instances of "Array_With_Length_Padded" are equal.
 */
void assert_padded_arrays_equality(struct Array_With_Length_Padded* first_padded_array,
                                    struct Array_With_Length_Padded* second_padded_array);

#endif // ARRAY_UTILITIES_H

