#version 140

uniform mat4 uni_model;
uniform mat4 uni_view;
uniform mat4 uni_proj;
uniform float uni_time;
uniform float uni_curvature;
uniform float uni_drugs;

in vec3 in_position;
in vec4 in_color;

out vec4 ex_color;

void main(void) {
	gl_Position = uni_proj * uni_view * uni_model *vec4(in_position, 1.0);
	gl_Position.y += (uni_curvature * ((gl_Position.w * gl_Position.w)+(gl_Position.x * gl_Position.x)-1.0));

	ex_color = in_color;

	/*
	gl_Position = uni_proj * uni_view * vec4(in_position, 1.0);
	ex_color = in_color;
	*/
}
