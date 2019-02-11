#version 140

uniform mat4 uni_model;
uniform vec3 uni_model_pos;
uniform mat4 uni_view;
uniform vec3 uni_view_dir;
uniform vec3 uni_view_pos;
uniform mat4 uni_proj;
uniform float uni_shade;
uniform float uni_time;
uniform float uni_curvature;
uniform float uni_drugs;

in vec3 in_position;
in vec4 in_color;
in vec3 in_normal;

out vec4 ex_color;

void main(void) {
	vec4 model_pos = (uni_model * vec4(in_position, 1.0)) + vec4(uni_model_pos, 0.0);
	vec4 view_pos = uni_view * model_pos;

	gl_Position = uni_proj * view_pos;
	gl_Position.y += (uni_curvature * ((gl_Position.w * gl_Position.w)+(gl_Position.x * gl_Position.x)-1.0));

	vec3 diff = normalize((uni_model * vec4(in_normal, 1.0)).xyz);
	float shade_amount = (dot(diff, uni_view_dir) * uni_shade) - uni_shade;
	ex_color = in_color + vec4(vec3(shade_amount), 1.0);
}
