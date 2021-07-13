.PHONY:clean

matrix: matrix.cpp test.cpp
	clang++ test.cpp matrix.cpp -o $@ -Wall -Wextra

clean:
	rm -f matrix
