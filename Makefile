.PHONY:clean, build_src
CC=clang++

# With this flag you can passs extra arguments for example debug (-g)
CEXTRA:=-O2

CFLAGS:=$(CEXTRA) -march=haswell -std=c++20 -Wall -Wextra -fno-exceptions
CFLAGS_DBG:=-g -march=haswell -std=c++20 -Wall -Wextra

LDFLAGS:=

# find all sources in src folder
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))
OBJS_DBG=$(patsubst src/%.cpp, obj_dbg/%.o, $(SRCS))

TESTS_SRC=$(wildcard tests/*.cpp)
TESTS=$(TESTS_SRC:.cpp=.out)
all: obj test

#build objecets
build_src: obj $(OBJS)
build_src_dbg: obj_dbg $(OBJS_DBG)

# build exectuable
test: $(OBJS) test.cpp
	$(CC) -Iheader $^ $(LDFLAGS) -o $@

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
