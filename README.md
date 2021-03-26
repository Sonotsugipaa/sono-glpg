# Sonotsugipaa's OpenGL Playground

<p> <table>
	<tr> <th> <strong>IMPORTANT</strong>: </th> </tr>
	<tr> <td>
		This project is dead, I have deleted some of its dependencies
		(written by me) because they were so badly structured and
		implemented that they were barely even buildable, and they
		definitely were an embarassment. I'm keeping this repository
		so that I can salvage some code in the future, but if I am
		to work on the same concept (and I probably will) I'll do
		this from scratch. Not <i>that</i> Scratch, I'll use the
		Vulkan API.
	</td> </tr>
</table> </p> <p>
A hobby project for a simple knock-off of a Snake game,
<strong>Sneka3D</strong>.
</p> <p>
<strong>Sneka3D</strong> is inspired by a Nokia game called
<strong>Snakes</strong>.<br>
Conceptually, the game features moddable levels and objects, pre-defined small
and big levels as well as procedurally generated worlds;<br>
it's also supposed
to be low-weight, although that depends on its quality.
</p>


## Requirements

1. <code>g++</code>, the C++ compiler
2. GNU Make (<code>make</code>)
3. Additional OpenGL related packages (more info below)
4. Hardware that supports OpenGL 3


## Dependencies

- The ones mentioned in '<b>Requirements</b>'
- <b>OpenGL 3</b>, <b>GLEW</b>, <b>SDL2</b>
- <b>GLM</b> (only the headers, for vector and matrix operations)
- <b>Amscript2</b> library (https://github.com/Sonotsugipaa/Amscript2)


## Setup

<p>
First you need to install some dependencies: <i>GLEW</i> and
<i>SDL2</i>.
<br>
On <b>Debian</b>-based systems, install these packages:<br>

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
</p> <p>
The last dependency is <b>Amscript</b>: it can be built and installed
by following the instructions written in it's repository's <i>README.md</i>.
</p>


## Building the project

### How to build

Run the <code>make</code> command.

### Makefile setup
<p>
Simply running <code>make</code> compiles each file in <code>src/</code>
that ends with the <code>.cpp</code> extension to <code>build/</code>,
and links every file in <code>src/main/</code> to <code>bin/</code> using
<b>ALL</b> <code>.o</code> files from <code>build/</code>.
</p> <p>
<code>make bin/???</code> builds a single executable file.
</p>

Sometimes it may be necessary to recompile everything. In order to do that:

- <code>make clean</code> deletes all intermediate files;
- <code>make purge</code> deletes all intermediate <b>AND</b> executable files.

### Assets

<strong>Sneka3D</strong> (like any respectable game) uses a variety of assets
that are not compiled directly into the executable file.<br>
Most of them are <b>amscript</b>s.

#### Meshes

<p>
Meshes are binary files containing vertices.<br>
Each vertex contains <b>7</b> 32-bit little-endian signed integer numbers,
that are interpreted by <strong>Sneka3D</strong>'s asset loader as fixed-point
numbers by dividing them by 100 - for example, a distance of <b>1</b> tile
equals <b>100</b> in the mesh file.
</p> <p>
The values of the vertices are:<br><br>
<b>1</b> to <b>3</b>: XYZ offset coordinates;<br>
<b>4</b> to <b>6</b>: RGB color values;<br>
<b>7</b>: Color transparency.
</p> <p>
A triangle is formed by <b>3</b> vertices, and their visible face is the one
where the vertices are seen in a <b>counter-clockwise</b> order.
</p> <p>
To create a mesh, use the <code>amso</code> utility, included with the
<b>Amscript</b> library.
</p>


## Licenses

<p>
As of <i>2019 / 04 / 13</i>, the <strong>Sneka3D</strong> project is being
developed by a single person, whom, for this section, will be addressed as
"<b>me</b>" or "<b>I</b>" using the proper pronouns.<br>
</p> <p>
I am an amateur developer, a Computer Science student and, most importantly,
someone who has <b><i>not worked professionally</i></b> (or even in a team,
excluding assignments about bubble-sorts) <b><i>yet</i></b>.<br>
I don't know anything about licensing, other than "<i>it's important to have your
software licensed</i>".
</p> <p>
This repository is under the
<i><b>GNU LESSER GENERAL PUBLIC LICENSE (Version 3)</b></i>, a copy of which
is included in the (repo's) root directory.<br>
</p> <p>
Other than that, I don't know how to address, or manage, licenses derived from
the tools I used - as far as I know, this repository could be a violation of
human rights from a legal perspective.<br>
If you know anything about licensing more than I do (very likely) and are
willing to help me not get sued into oblivion, please contact me via email
(<i>mark.prl.97@gmail.com</i>) and tell me how to determine what other
open-source licenses I should be aware of, or how to properly address them.
</p>
