CPPFLAGS = $(shell cat compile_opts)
OPTS = $(shell cat make_opts)

# empty target
nothing:

# objects should not be removed automatically
.PRECIOUS: build/%.o

# external Amscript2 dependency
Amscript2/lib/libamscript2.a:
	git submodule update --init Amscript2
	make --directory="Amscript2" $(patsubst Amscript2/%,%,$@)

# links all C++ source files from ./src/main
bin/%: $(ALL_OBJS) src/main/%.cpp
	mkdir -p bin/
	#
	# ----- C++ executable ----- #
	g++ $(CPPFLAGS) -o"$@" $^ $(OPTS)

# compiles a C++ source file from ./src
build/%.o: src/%.cpp
	mkdir -p build/
	#
	# ----- C++ object ----- #
	g++ $(CPPFLAGS) $< -c -o $@

.PHONY: lib/libmodule_%.a
lib/libmodule_%.a: src/%/makefile
	make --makefile=$< $@

bin/asset.%: src/test/%.cpp lib/libmodule_asset.a lib/libmodule_util.a
	mkdir -p bin/
	g++ $(CPPFLAGS) $< -o $@ -lmodule_asset -lmodule_util

setup: purge
	#
	# ----- Workspace creation ----- #
	mkdir -p src/main include/sneka shader assets doc

clean:
	rm -rf build
	mkdir build

purge:
	rm -rf bin build Amscript2/lib
	mkdir bin build Amscript2/lib
