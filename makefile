COMMON_FLAGS = -g -pedantic -Wall -Wextra -Wpedantic -I./include \
               -I./Amscript2/include -L./Amscript2/lib
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

# external Amscript2 dependency
Amscript2/lib/libamscript2.a:
	if [ ! -f "Amscript2/makefile" ]; then git submodule update --init Amscript2; fi
	make --directory="Amscript2" $(patsubst Amscript2/%,%,$@)

# links all C source files from ./src/main
bin/%: $(ALL_OBJS) src/main/%.c
	mkdir -p bin/
	#
	# ----- C executable ----- #
	gcc $(CFLAGS) -o"$@" $^ $(OPTS)

# links all C++ source files from ./src/main
bin/%: $(ALL_OBJS) src/main/%.cpp
	mkdir -p bin/
	#
	# ----- C++ executable ----- #
	g++ $(CPPFLAGS) -o"$@" $^ $(OPTS)

# compiles a C source file from ./src
build/%.o: src/%.c Amscript2/lib/libamscript2.a
	mkdir -p build/
	#
	# ----- C object ----- #
	gcc $(CFLAGS) $< -c -o $@

# compiles a C++ source file from ./src
build/%.o: src/%.cpp Amscript2/lib/libamscript2.a
	mkdir -p build/
	#
	# ----- C++ object ----- #
	g++ $(CPPFLAGS) $< -c -o $@

setup:
	#
	# ----- Workspace creation ----- #
	mkdir -p src src/main build bin include
	printf '' >>libraries

clean:
	rm -rf build
	mkdir build

purge:
	rm -rf bin build Amscript2/lib
	mkdir bin build Amscript2/lib
