.PHONY:clean, tests
CC=clang++

# With this flag you can passs extra arguments for example debug (-g)
CEXTRA:=-O2

CFLAGS:=$(CEXTRA) -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions
LDFLAGS:=

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))

TESTS_SRC=$(wildcard tests/*.cpp)
TESTS=$(TESTS_SRC:.cpp=.out)
all: obj test

tests: $(TESTS)


# build all tests
$(TESTS): %.out:%.cpp $(OBJS)
	clang++ $< -Iheader $(OBJS) -lbenchmark -pthread -o $@ -std=c++20 -Wall -Wextra


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
