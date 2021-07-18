.PHONY:clean
CC=clang++
CFLAGS:=-O2 -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions
LDFLAGS:=

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)


matrix: test.cpp $(SRCS)
	clang++ $(CFLAGS) -Iheader $^ -o $@

clean:
	rm -f matrix
