
main_c_file := $(wildcard *.c)
main_prog_file = qsort_bitonic_compare

all: $(main_c_file)
	gcc -g -O3 -o $(main_prog_file) $? $(CPPFLAGS) -lm -lOpenCL $(LDFLAGS)

clean:
	rm -f $(main_prog_file)

