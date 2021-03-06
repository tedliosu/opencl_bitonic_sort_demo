
# Instructions

1. Setup OpenCL development on your machine appropriately.
   1. Refer to [these set of instructions](https://github.com/tedliosu/opencl_install_instructions)
     if **running a Linux distro** on an AMD GPU and/or any kind of CPU.
   2. Next (if on a Linux distro), make sure your "CPPFLAGS" and
       "LDFLAGS" environment variables are set appropriately. (e.g.
       CPPFLAGS="-I/opt/rocm/opencl/include" LDFLAGS="-L/opt/rocm/opencl/lib").

2. Git clone this repository

3. Change working directory to cloned repository

4. Run "make all"

5. The resulting executable will pit the parallel OpenCL version of bitonic sort against the serial version
   of bitonic sort and the C standard library's Qsort, and report the time it took to sort a certain number
   of elements for each sorting algorithm. Given each sorting procedure is sorting copies of the same array,
   the executable will check the sorted results of each version of bitonic sort against the sorted results
   of Qsort to see if the sorting was performed correctly by each version of bitonic sort. A message of
   congratulations appears for each version of bitonic sort if said version of bitonic sort sorted the array
   correctly.

6. You may tweak the ARRAY_LEN macro value in "qsort_bitonic_compare.h", the ARRAY_TYPE macro value in
   "naive_bitonic_sort_opencl.h", the NUM_THREADS_IN_BLOCK macro value in "naive_bitonic_sort_opencl.h",
   and the SORTING_DIRECTION macro value in "naive_bitonic_sort_opencl.h" to see the executable perform
   parallelized bitonic sort in OpenCL, serial bitonic sort in C, and qsort in C with different types of
   data with various different parameters.
    - **IMPORTANT NOTE**: the value of the NUM_THREADS_IN_BLOCK macro in "naive_bitonic_sort_opencl.h"
      must be a value such that the smallest power of 2 larger than the value of ARRAY_LEN in "qsort_bitonic_compare.h"
      is divisible by the value of NUM_THREADS_IN_BLOCK.  For example, if the value of ARRAY_LEN is 100,
      the value of NUM_THREADS_IN_BLOCK must be 128 or a power of 2 smaller than 128.  **If the value
      of NUM_THREADS_IN_BLOCK doesn't meet the divisibility requirement specified here, the main C program
      WILL SEGFAULT.**

7. You may also adjust the DESIRED_PLATFORM_INDEX macro value in "qsort_bitonic_compare.h" for running
   parallelize bitonic sort in OpenCL on different OpenCL platforms on your machine. However, **if you
   have only 1 OpenCL platform installed on your machine, you MUST set DESIRED_PLATFORM_INDEX to 0.**

# Comments about code in general

 - Please see code comments in "naive_bitonic_sort_opencl.h" near top of file for web pages I gathered info
   from for implementing bitonic sort in OpenCL and implementing in regular C.

# Performance Metrics

## Program Output With Program Running OpenCL Using GPU ##

### On Machine With A Ryzen 5 2600 And A RX Vega 56; Array Length 100 Million ###

<pre>
>>> OpenCL program compiler result message: - 

>>> Starting OpenCL parallelized bitonic sorting of doubles with 256 work-items per workgroup, sort descending...
Parallelized bitonic sort of 100000000 element(s) on OpenCL device took 2.494582 seconds

>>> Starting sorting doubles with serial bitonic sort on CPU, sort descending...
Serial bitonic sort on CPU of 100000000 element(s) in main memory took 62.986346 seconds

>>> Starting sorting of doubles with qsort on CPU, sort descending...
Qsort on CPU of 100000000 element(s) in main memory took 18.480286 seconds

>>> Verifying correctness of parallelized bitonic sort on OpenCL device...
Congratulations, both of your padded arrays are identical in content!
>>> Verifying correctness of serial bitonic sort in main memory...
Congratulations, both of your padded arrays are identical in content!
</pre>

### On Machine With A Ryzen 5 2600 And A RX Vega 56; Array Length 500 Million ###

<pre>
>>> OpenCL program compiler result message: - 

>>> Starting OpenCL parallelized bitonic sorting of doubles with 256 work-items per workgroup, sort descending...
Parallelized bitonic sort of 500000000 element(s) on OpenCL device took 11.263392 seconds

>>> Starting sorting doubles with serial bitonic sort on CPU, sort descending...
Serial bitonic sort on CPU of 500000000 element(s) in main memory took 300.024577 seconds

>>> Starting sorting of doubles with qsort on CPU, sort descending...
Qsort on CPU of 500000000 element(s) in main memory took 105.718650 seconds

>>> Verifying correctness of parallelized bitonic sort on OpenCL device...
Congratulations, both of your padded arrays are identical in content!
>>> Verifying correctness of serial bitonic sort in main memory...
Congratulations, both of your padded arrays are identical in content!
</pre>

# TODOs

 - Unit testing of both versions of bitonic sort with edge cases (e.g. array length of 1)
   and sorting duplicate numbers in an array.

