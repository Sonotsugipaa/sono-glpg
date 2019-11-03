CPPFLAGS = $(shell cat compile_opts)

build/asset/%.o: src/asset/%.cpp
	mkdir -p build/asset
	g++ $(CPPFLAGS) -c -o "$@" "$<"

wbuild/asset/%.o: src/asset/%.cpp
	mkdir -p wbuild/asset
	g++ $(CPPFLAGS) -c -o "$@" "$<"

lib/libmodule_asset.a: $(patsubst src/%.cpp, build/%.o, $(wildcard src/asset/*.cpp))
	mkdir -p lib
	ar -rs $@ $^

wlib/libmodule_asset.a: $(patsubst src/%.cpp, wbuild/%.o, $(wildcard src/asset/*.cpp))
	mkdir -p wlib
	ar -rs $@ $^
