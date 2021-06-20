

// Necessary libraries and headers for functions
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "array_utilities.h"


struct Array_With_Length_Padded *get_rand_padded_array(const unsigned int array_len) {

   // Array length has to be greater than zero
   assert(array_len > 0);
   // Variable declarations
   struct timespec curr_time_info;
   struct Array_With_Length_Padded* array_w_len_padded = malloc(sizeof(*array_w_len_padded));
   unsigned int padded_2n_length = (unsigned int) exp2(ceil(log2(array_len))); 
   // Edge case handling where array_len = 1
   if (array_len == 1) {
        ++padded_2n_length;
   }
   const float range_min_val = -1.0f;
   const float range_interval = 2.0f;
   const int even_test_num = 2;
   array_w_len_padded->array_len_actual = array_len;
   array_w_len_padded->padded_2n_length = padded_2n_length;
   array_w_len_padded->padding_location_indicator = PAD_ARRAY_AT_END;
  
   // Retrieve current system nanoseconds and use as seed for random array generation 
   timespec_get(&curr_time_info, TIME_UTC);
   srand(curr_time_info.tv_nsec);

   /*
    * Generate the appropriate random array of characters/numbers and then
    * pad the array with extra depending on the ARRAY_TYPE
    */
  #if(ARRAY_TYPE == CHAR)
     
     char* rand_char_array = malloc(padded_2n_length * sizeof(*rand_char_array));

     for (unsigned int curr_entry = 0; curr_entry < array_len; ++curr_entry) {
        rand_char_array[curr_entry] = (char) (rand() % UCHAR_MAX);    
     }
     for (unsigned int curr_entry = array_len; curr_entry < padded_2n_length; ++curr_entry) {
        rand_char_array[curr_entry] = CHAR_MAX;    
     }
     
     array_w_len_padded->contents = rand_char_array;
   
   #elif (ARRAY_TYPE == INT)
     
     int* rand_int_array = malloc(padded_2n_length * sizeof(*rand_int_array));

     for (unsigned int curr_entry = 0; curr_entry < array_len; ++curr_entry) {
        rand_int_array[curr_entry] = (int) ((range_min_val + ((double) rand() * range_interval
                                                                              / (double) RAND_MAX))
                                                                                 * (double) UINT_MAX);
       // compensation for above algorithm generating only even numbers
       if (rand() % even_test_num != 0) {
          ++rand_int_array[curr_entry];
       }
     }
     for (unsigned int curr_entry = array_len; curr_entry < padded_2n_length; ++curr_entry) {
        rand_int_array[curr_entry] = INT_MAX;    
     }
     
     array_w_len_padded->contents = rand_int_array;

   #elif (ARRAY_TYPE == LONG)
     
     long* rand_long_array = malloc(padded_2n_length * sizeof(*rand_long_array));

     for (unsigned int curr_entry = 0; curr_entry < array_len; ++curr_entry) {
        rand_long_array[curr_entry] = (long) ((range_min_val + ((double) rand() * range_interval
                                                                              / (double) RAND_MAX))
                                                                                * (double) ULONG_MAX);    
       // compensation for above algorithm generating only even numbers
       if (rand() % even_test_num != 0) {
          ++rand_long_array[curr_entry];
       }
     }
     for (unsigned int curr_entry = array_len; curr_entry < padded_2n_length; ++curr_entry) {
        rand_long_array[curr_entry] = LONG_MAX;    
     }
     
     array_w_len_padded->contents = rand_long_array;
   
   #elif (ARRAY_TYPE == FLOAT)
     
     float* rand_float_array = malloc(padded_2n_length * sizeof(*rand_float_array));

     for (unsigned int curr_entry = 0; curr_entry < array_len; ++curr_entry) {
        rand_float_array[curr_entry] = range_min_val + ((float) rand() * range_interval / (float) RAND_MAX);    
     }
     for (unsigned int curr_entry = array_len; curr_entry < padded_2n_length; ++curr_entry) {
        rand_float_array[curr_entry] = FLT_MAX;    
     }
     
     array_w_len_padded->contents = rand_float_array;
   
   #elif (ARRAY_TYPE == DOUBLE)
     
     double* rand_double_array = malloc(padded_2n_length * sizeof(*rand_double_array));

     for (unsigned int curr_entry = 0; curr_entry < array_len; ++curr_entry) {
        rand_double_array[curr_entry] = range_min_val + ((double) rand() * range_interval / (double) RAND_MAX);    
     }
     for (unsigned int curr_entry = array_len; curr_entry < padded_2n_length; ++curr_entry) {
        rand_double_array[curr_entry] = DBL_MAX;    
     }
     
     array_w_len_padded->contents = rand_double_array;
   
   #endif
    
   return array_w_len_padded;

}


struct Array_With_Length_Padded *deep_cp_padded_array(struct Array_With_Length_Padded* padded_array) {

   // No null pointers allowed for parameter
   assert(padded_array != NULL);

   // Allocate memory for deep copy of function parameter
   const unsigned int array_with_padding_len = padded_array->padded_2n_length;
   struct Array_With_Length_Padded* padded_array_deep_cp = malloc(sizeof(*padded_array_deep_cp));
   padded_array_deep_cp->contents = malloc(array_with_padding_len * sizeof(*(padded_array_deep_cp->contents)));

   // Copy over non-pointer fields' values
   padded_array_deep_cp->array_len_actual = padded_array->array_len_actual;
   padded_array_deep_cp->padded_2n_length = array_with_padding_len;
   padded_array_deep_cp->padding_location_indicator = padded_array->padding_location_indicator;

   // Copy over contents of array
   memcpy(padded_array_deep_cp->contents, padded_array->contents,
                              array_with_padding_len * sizeof(*(padded_array->contents)));

   return padded_array_deep_cp;

}

void print_array(struct Array_With_Length_Padded* array) {

   // No null pointers allowed for parameter
   assert(array != NULL);

   unsigned int array_index_begin = 0;
   unsigned int array_index_end = array->array_len_actual;

   if (array->padding_location_indicator) {
      array_index_begin = array->padded_2n_length - array->array_len_actual;
      array_index_end = array->padded_2n_length;
   }

   for (unsigned int array_index = array_index_begin; array_index < array_index_end; ++array_index) {
      printf(DISPLAY_FORMAT_STR, array->contents[array_index]);
   }

   printf("\n");

}

void assert_padded_arrays_equality(struct Array_With_Length_Padded* first_padded_array,
                                     struct Array_With_Length_Padded* second_padded_array) {

     assert(first_padded_array != NULL);
     assert(second_padded_array != NULL);
     assert(first_padded_array->array_len_actual == second_padded_array->array_len_actual);
     assert(first_padded_array->padded_2n_length == second_padded_array->padded_2n_length);
     assert(first_padded_array->padding_location_indicator == second_padded_array->padding_location_indicator);

     for (unsigned int curr_index = 0; curr_index < first_padded_array->padded_2n_length; ++curr_index) {
          assert(first_padded_array->contents[curr_index] == second_padded_array->contents[curr_index]);
     }

     printf(ASSERTION_PASSED_INFORM_USER);

}

