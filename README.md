
# Instructions

1. Setup OpenCL development on your machine appropriately; if on a Linux distro, make sure your
   "CPPFLAGS" and "LDFLAGS" environment variables are set appropriately. (e.g. CPPFLAGS="-I/opt/rocm/opencl/include"
    LDFLAGS="-L/opt/rocm/opencl/lib").

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
   data with various different parameters. You may also adjust the DESIRED_PLATFORM_INDEX macro value in
   "qsort_bitonic_compare.h" for running parallelize bitonic sort in OpenCL on different OpenCL platforms
   on your machine. However, if you have only 1 OpenCL platform installed on your machine, you MUST
   set DESIRED_PLATFORM_INDEX to 0.

# Comments about code in general

 - Please see code comments in "naive_bitonic_sort_opencl.h" near top of file for web pages I gathered info
   from for implementing bitonic sort in OpenCL and implementing in regular C.

# TODOs

 - Unit testing of both versions of bitonic sort with edge cases (e.g. array length of 1)
   and sorting duplicate numbers in an array.

