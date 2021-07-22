.PHONY:clean
CC=clang++

# With this flag you can passs extra arguments for example debug (-g)
CEXTRA:=-O2

CFLAGS:=$(CEXTRA) -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions
LDFLAGS:=

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))

all: obj test

# build exectuable
test: $(OBJS) test.cpp
	$(CC) -Iheader $^ $(LDFLAGS) -o $@

# build all objects
$(OBJS) : obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) -Iheader $< -o $@ -c

# directory used for storing object files
obj:
	@mkdir -p obj

# clean intermidiate files
clean:
	rm obj/*.o
	rm -f test
