# Sonotsugipaa's OpenGL Playground

<p>
Small workspace to play around with OpenGL.
</p> <p>
This project exists for the sake of a knock-off version of an old 3D Snake
from Nokia.
<br>
Also for learning OpenGL.
<br>
Now that I think about it, I'm not breaching any copyright gizmo... am I?
</p>


# Requirements

1. `g++`, the C++ compiler
2. GNU Make (`make`)
3. Additional OpenGL related packages (more info below)
4. Hardware that supports OpenGL 3


# Dependencies

- The ones mentioned in '<b>Requirements</b>'
- OpenGL 3, GLEW, SDL2
- GLM (only the headers, for vector and matrix operations)
- Git (obviously)


# Setup

<p>
First you need to install some dependencies: <i>OpenGL</i>, <i>GLEW</i> and
<i>SDL2</i>.
<br>
On Ubuntu (and derivates), install these packages:

		sudo apt-get update
		sudo apt-get install libsdl2-dev libgl1-mesa-dev libx11-dev libxrandr-dev

</p> <p>
Next, you need the GLM headers. The most space-efficient way to do so is to
download the latest version (https://github.com/g-truc/glm/releases/tag/0.9.9.3)
and extract the header files, found in the folder `glm/glm/`, to `/usr/include`:
to do so, temporarily extract the `glm/glm/` folder (<i>NOT THE PARENT</i>, `glm/`!),
open a terminal where you extracted the folder and enter
`sudo mv ./glm /usr/include/.` .
<br>
If everything went according to my plan, you now have the file
`/usr/include/glm/glm.hpp`, among others; you're going to have a stray
`CMakeLists.txt`, you can safely remove that - at least for the scope of
this workspace.
</p> <p>
That's it, for now.
</p>


# Usage

<p>
To create an executable program, create a source file (ending with `*.cpp`) in
the `src/main/` folder - this file must contain the main function.
If you want to create intermediate objects, create and write source files in
`src/` with the aforementioned file extension, and it will automatically be
compiled to `build/` and used when compiling executables, which can be found
in `bin/`.<br>

You will be able to #include everything in `include/`.
</p>

<p>
To compile and link everything, simply run `make make_exec` (or just `make`).
<br>
To clear compiled intermediate files, run `make clear`; if you want to delete
executables too, run `make purge`.
<br>
To compile single executables or intermediate files, run `make bin/NAME` or
`make build/NAME`, where 'NAME' is (obviously) the name of the file you want
to compile/link.
</p>


# License

I will be frank: I have no idea how to use, manage or understand licences.
This is amateur code, I'm not going to sue anyone who uses any of it;
just be aware of the licensed libraries and tools you (and I) are going to use,
which are all available for free for non-commercial use.
<br>
Most of those have been mentioned in this file, excluding Linux and what not.
