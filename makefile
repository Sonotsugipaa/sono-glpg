COMMON_FLAGS = -g -pedantic -Wall -Wextra -Wpedantic -I./include
CFLAGS = -std=gnu99 $(COMMON_FLAGS)
CPPFLAGS = -std=gnu++17 $(COMMON_FLAGS)
ALL_MAIN_SRCS =\
	$(patsubst src/main/%.c, bin/%, $(wildcard src/main/*.c))\
	$(patsubst src/main/%.cpp, bin/%, $(wildcard src/main/*.cpp))
C_SRCS = $(wildcard src/*.c)
CPP_SRCS = $(wildcard src/*.cpp)
ALL_OBJS =\
	$(patsubst src/%.c, build/%.o, $(C_SRCS))\
	$(patsubst src/%.cpp, build/%.o, $(CPP_SRCS))
OPTS = $(shell cat make_opts)

# compiles all objects, and creates executable files from ./src/main
make_exec: $(ALL_MAIN_SRCS)

# objects should not be removed automatically
.PRECIOUS: build/%.o

# links all C source files from ./src/main
bin/%: $(ALL_OBJS) src/main/%.c
	#
	# ----- C executable ----- #
	gcc $(CFLAGS) -o"$@" $^ $(OPTS)

# links all C++ source files from ./src/main
bin/%: $(ALL_OBJS) src/main/%.cpp
	#
	# ----- C++ executable ----- #
	g++ $(CPPFLAGS) -o"$@" $^ $(OPTS)

# compiles a C source file from ./src
build/%.o: src/%.c
	#
	# ----- C object ----- #
	gcc $(CFLAGS) $< -c -o $@

# compiles a C++ source file from ./src
build/%.o: src/%.cpp
	#
	# ----- C++ object ----- #
	g++ $(CPPFLAGS) $< -c -o $@

bin/baker: src/main/baker.cpp
	#
	# ----- .src processor ----- #
	g++ $(CPPFLAGS) -o"$@" $^ $(OPTS)

# compiles decimal-to-binary converter
bin/itob: src/main/itob.cpp build/read_utils.o
	#
	# ----- 10-to-2 converter ----- #
	g++ $(CPPFLAGS) -o"$@" $^ $(OPTS)


%: %.bake bin/itob bin/baker
	$(shell ./make_asset "$<" "$@")

setup:
	#
	# ----- Workspace creation ----- #
	mkdir -p src src/main build bin include
	printf '' >>libraries

clean:
	rm -rf build
	mkdir build

purge:
	rm -rf bin build
	mkdir bin build
