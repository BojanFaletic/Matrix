.PHONY:clean, build_src, tests, unittest, build_src_dbg
CC=clang++

# With this flag you can passs extra arguments for example debug (-g)
CEXTRA:=-O2

CFLAGS:=$(CEXTRA) -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions -DNDEBUG
CFLAGS_DBG:=-g -march=haswell -std=c++20 -Wall -Wextra

LDFLAGS:= -std=c++20

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))
OBJS_DBG=$(patsubst src/%.cpp, obj_dbg/%.o, $(SRCS))

TESTS_SRC=$(wildcard tests/*.cpp)
TESTS=$(TESTS_SRC:.cpp=.out)
all: obj test

# build tests
TESTS_SRC=$(wildcard tests/*.cpp)
TESTS=$(TESTS_SRC:.cpp=.out)
tests: $(TESTS)

# build testbench
UNITTEST_SRC=$(wildcard unittest/*.cpp)
UNITTESTS=$(UNITTEST_SRC:.cpp=.out)
unittest: $(UNITTESTS)

# build test with fixed matrix
test_fixed: test.cpp header/matrix_fixed.hpp
	clang++ test.cpp -Iheader -std=c++20 -O2 -o test_fixed


# build all tests
$(TESTS): %.out:%.cpp $(OBJS)
	clang++ $< -Iheader $(OBJS) -lbenchmark -pthread -o $@ -std=c++20 -Wall -Wextra -O3


# build unit tests
$(UNITTESTS): %.out:%.cpp $(OBJS)
	clang++ $< -Iheader $(OBJS) -lgtest -o $@ -std=c++20 -Wall -Wextra -O0


#build objecets
build_src: obj $(OBJS)
build_src_dbg: obj_dbg test_dbg

# build exectuable
test: $(OBJS) test.cpp
	$(CC) -Iheader $^ $(LDFLAGS) -o $@

test_dbg: $(OBJS_DBG) test.cpp
	$(CC) -Iheader $^ $(LDFLAGS) -g -o $@


# build all objects
$(OBJS) : obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) -Iheader $< -o $@ -c

$(OBJS_DBG) : obj_dbg/%.o : src/%.cpp
	$(CC) $(CFLAGS_DBG) -Iheader $< -o $@ -c

# directory used for storing object files
obj:
	@mkdir -p obj

obj_dbg:
	@mkdir -p obj_dbg

# clean intermidiate files
clean:
	rm obj/*.o
	rm obj_dbg/*.o
	rm -f test
