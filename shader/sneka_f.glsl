#version 140

// ... wat
precision highp float;

uniform vec4 uni_mul_col;
uniform vec4 uni_fog;

in vec3 screen_position;
in vec4 ex_color;

out vec4 out_color;

void main(void) {
	float fog_intensity = exp(-uni_fog.w / screen_position.z);
	out_color = ex_color * uni_mul_col;
	out_color.rgb = ((1-fog_intensity) * uni_fog.rgb) + (fog_intensity * out_color.rgb);
}
