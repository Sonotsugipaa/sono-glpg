#version 300 es
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec4 ex_Color;
out vec4 out_color;

void main(void) {
    out_color = ex_Color;
}
