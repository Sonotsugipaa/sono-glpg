#version 300 es

// ... wat
precision highp float;

uniform vec4 uni_add_col;

in vec4 ex_color;

out vec4 out_color;

void main(void) {
    out_color = ex_color;
}
