# Sonotsugipaa's OpenGL Playground

<p>
Small workspace to play around with OpenGL.
</p> <p>
This project exists for the sake of a knock-off version of an old 3D Snake
from Nokia.
<br>
Also for learning OpenGL.
</p>


# Requirements

1. <code>g++</code>, the C++ compiler
2. GNU Make (<code>make</code>)
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
On Ubuntu (and derivates), install these packages:<br>

<code>sudo apt-get update</code><br>
<code>sudo apt-get install libsdl2-dev libgl1-mesa-dev libx11-dev libxrandr-dev</code><br>


</p> <p>
Next, you need the GLM headers. The most space-efficient way to do so is to
download the latest version (https://github.com/g-truc/glm/releases/tag/0.9.9.3)
and extract the header files, found in the folder <code>glm/glm/</code>, to
<code>/usr/include</code>: to do so, temporarily extract the
<code>glm/glm/</code> folder (<i>NOT THE PARENT</i>, <code>glm/</code>!), open
a terminal where you extracted the folder and enter
<code>sudo mv ./glm /usr/include/.</code>.
<br>
If everything went according to my plan, you now have the file
<code>/usr/include/glm/glm.hpp</code>, among others; you're going to have a
stray <code>CMakeLists.txt</code>, you can safely remove that - at least for
the scope of this workspace.
</p> <p>
That's it, for now.
</p>


# Usage

<p>
To create an executable program, create a source file
(ending with <code>.cpp</code>) in the <code>src/main/</code> folder - this
file must contain the main function.
If you want to create intermediate objects, create and write source files in
<code>src/</code> with the aforementioned file extension, and it will
automatically be compiled to <code>build/</code> and used when compiling
executables, which can be found in <code>bin/</code>.<br>

You will be able to #include everything in <code>include/</code>.
</p>

<p>
To compile and link everything, simply run <code>make make_exec</code> (or just <code>make</code>).
<br>
To clear compiled intermediate files, run <code>make clear</code>; if you want to delete
executables too, run <code>make purge</code>.
<br>
To compile single executables or intermediate files, run <code>make bin/NAME</code> or
<code>make build/NAME</code>, where 'NAME' is (obviously) the name of the file you want
to compile/link.
</p>

### Assets

<p>
The workspace includes some utility programs for creating binary files from
decimal numbers, which should be stored in assets.<br>
Those utilities, compiled (like everything else) to <code>bin/</code> are:
</p>

1. itob <i>(<b>I</b>nteger <b>TO</b> <b>B</b>inary)</i>
2. baker <i>(<b>B</b>inary m<b>AKER</b>)</i>

<p>
Itob simply converts all of its arguments to binary characters, and prints
them to the output stream:
<code>itob 97 98</code> will print <code>a...b...</code> (each period being
the null character, ASCII 0).
</p> <p>
Baker simply prints every argument to the output stream, but allows the
definition and replacement of character sequences.
</p>

- If an argument starts with <code>!</code>, a variable with that name (without
  <code>!</code>) will be defined, for example<br>
  <code>baker !abc STR</code><br>
  will define <code>abc</code> as <code>STR</code> (without actually printing
  anything);
- If an argument starts with <code>?</code>, the variable with that name will
  be used instead (unless it hasn't been defined), so<br>
  <code>baker !abc STR Hello, ?abc ?undefined</code><br>
  will print <code>Hello, STR</code>.
- If a variable definition starts with <code>[</code> and ends with
  <code>]</code>, everything between the tokens will be the variable's value.<br>
  <b>IMPORTANT NOTE</b>: the closing bracket does <b>NOT</b> delimit a token,
  and an argument such as <code>ABC]DE</code> will not end the variable's
  definition. This may be useful for defining variables with brackets as
  characters, such as <code>!key [[value1 value2]]</code> so that
  <code>?key</code> expands to <code>[value1 value2]</code>, brackes included
  (again, a space between the two <b>closing</b> brackets will result in the
  end of the variable definition on the first one).

<p>
The argument following a variable definition
(<i>arg2</i> in <code>!arg1 arg2</code>) does not expand, therefore<br>
<code>baker !! ! ?!</code> will print <code>!</code>, and<br>
<code>baker !? ? ??</code> will print <code>?</code>.<br>
These two examples are actually built-in variables.
</p> <p>
It is possible to combine the two programs, "compiling" a binary file using
the GNU Make target <code>%: %.bake [...]</code>.
</p>


# Licenses

GNU Lesser General Public License<br>
<i>(included in the repository)</i>

### GNU General Public License v3

From GNU Make, G++:<br>
<i>https://www.gnu.org/licenses/gpl-3.0.en.html</i>

### GNU General Public License v2

From Git;<br>
<i>https://www.gnu.org/licenses/gpl-2.0.en.html</i>

### zlib License

From SDL2:<br>
<i>https://www.zlib.net/zlib_license.html</i>

### Modified BSD License

From GLEW:<br>
<i>http://glew.sourceforge.net/glew.txt</i>

### Mesa 3-D License

From GLEW:<br>
<i>http://glew.sourceforge.net/mesa.txt</i>

### Khronos License

From GLEW:<br>
<i>http://glew.sourceforge.net/khronos.txt</i>

### Other Licenses

I could not find the exact license used by OpenGL, although
it *is* an open-source license, and this whole project is
completely open-source.
