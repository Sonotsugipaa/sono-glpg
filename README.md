# Sonotsugipaa's OpenGL (pg...?)

Small workspace to play around with OpenGL.<br>
**[ description under construction ]**


# Requirements

1. A GNU compiler (`gcc` for C, `g++` for C++)
2. GNU Make (`make`)


# Setup

First you need to install some dependencies: _OpenGL_, _GLEW_ and _SDL2_.<br>
On Ubuntu (and derivates), install these packages:
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libgl1-mesa-dev libx11-dev libxrandr-dev
```
Done.


# Usage

<p>
To create an executable program, create a source file (ending with either `*.c`
or `*.cpp`) in the `src/main/` folder - this file must contain the main
function.
If you want to create intermediate objects, create and write source files in
`src/` with the aforementioned file extension, and it will automatically be
compiled to `build/` and used when compiling executables, which can be found
in `bin/`.<br>

You will be able to #include everything in `include/`.

</p> <p>

To compile and link everything, simply run `make make_exec` (or just `make`).<br>

To clear compiled intermediate files, run `make clear`; if you want to delete
executables too, run `make purge`.<br>

To compile single executables or intermediate files, run `make bin/NAME` or
`make build/NAME`, where _NAME_ is (obviously) the name of the file you want
to compile/link.
</p>


# License

I will be frank: I have no idea how to use, manage or understand licences.
This is amateur code, I'm not going to sue anyone who uses any of my code;
just be aware of the licensed libraries and tools you (and I) are going to use,
which are all available for free for non-commercial use.


# Resources

<p>
OpenGL / SDL tutorial<br>
http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
</p>
