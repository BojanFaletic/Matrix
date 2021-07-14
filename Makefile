.PHONY:clean

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)

matrix: $(SRCS)
	clang++ -Iheader test.cpp $^ -o $@ -Wall -Wextra -O2 -march=haswell

clean:
	rm -f matrix
