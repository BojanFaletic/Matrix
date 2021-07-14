.PHONY:clean

matrix: matrix.cpp test.cpp benchmark.cpp
	clang++ test.cpp benchmark.cpp matrix.cpp -o $@ -Wall -Wextra -O2 -march=haswell

clean:
	rm -f matrix
