.PHONY:clean

matrix:matrix.cpp
	clang++ $< -o $@ -Wall -Wextra

clean:
	rm -f matrix
