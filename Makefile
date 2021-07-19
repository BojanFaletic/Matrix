.PHONY:clean, test
CC=clang++

# With this flag you can passs extra arguments for example debug (-g)
CEXTRA:=-O2

CFLAGS:=$(CEXTRA) -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions
LDFLAGS:=

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)


matrix: test.cpp $(SRCS)
	$(CC) $(CFLAGS) -Iheader $^ -o $@

test: test.cpp $(SRCS)
	$(CC) $(CFLAGS) -Iheader $^ -o $@

clean:
	rm -f matrix
