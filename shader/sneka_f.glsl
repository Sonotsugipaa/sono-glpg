#version 130

// ... wat
precision highp float;

uniform vec4 uni_mul_col;
uniform vec4 uni_fog;

in vec3 screen_position;
in vec4 ex_color;
in vec3 ex_reflect;
in vec3 ex_absorb;

out vec4 out_color;

void main(void) {
	float fog_intensity = exp(-uni_fog.w / screen_position.z);
	out_color = (ex_color * uni_mul_col) + vec4(ex_reflect, 1.0) + vec4(ex_absorb, 1.0);
	out_color.rgb = ((1-fog_intensity) * uni_fog.rgb) + (fog_intensity * out_color.rgb);
	/*
	out_color = ex_color;
	out_color.rgb += 10.5;
	*/
}
