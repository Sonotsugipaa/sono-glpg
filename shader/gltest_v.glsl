#version 300 es

uniform mat4 uni_trans;
uniform mat4 uni_proj;
in vec3 in_position;
in vec4 in_color;

out vec4 ex_color;

void main(void) {
	gl_Position = uni_proj * uni_trans * vec4(in_position, 1.0);
	ex_color = in_color;
}
