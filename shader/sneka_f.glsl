#version 140

// ... wat
precision highp float;

uniform vec4 uni_mul_col;

in vec4 ex_color;

out vec4 out_color;

void main(void) {
	out_color = ex_color * uni_mul_col;
}
