CPPFLAGS = $(shell cat compile_opts)

build/util/%.o: src/util/%.cpp
	@mkdir -p build/util
	g++ $(CPPFLAGS) -c -o "$@" "$<"

wbuild/util/%.o: src/util/%.cpp
	@mkdir -p wbuild/util
	g++ $(CPPFLAGS) -c -o "$@" "$<"

lib/libmodule_util.a: $(patsubst src/%.cpp, build/%.o, $(wildcard src/util/*.cpp))
	@mkdir -p lib
	ar -rs $@ $^

wlib/libmodule_util.a: $(patsubst src/%.cpp, wbuild/%.o, $(wildcard src/util/*.cpp))
	@mkdir -p wlib
	ar -rs $@ $^
