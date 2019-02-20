#version 130

uniform mat4 uni_model;
uniform mat4 uni_view;
uniform mat4 uni_proj;
uniform vec3 uni_model_pos;
uniform vec3 uni_light_dir;
uniform vec3 uni_view_pos;
uniform float uni_shade;
uniform float uni_reflect;
uniform float uni_reflect_falloff;
uniform float uni_time;
uniform float uni_curvature;
uniform float uni_drugs;

in vec3 in_position;
in vec4 in_color;
in vec3 in_normal;

out vec4 ex_color;
out vec3 screen_position;

void main(void) {
	float fac1 = 1.317 * uni_drugs;
	float fac2 = 0.757 * uni_drugs;
	float fac3 = 2.257 * uni_drugs;

	vec4 model_pos = (uni_model * vec4(in_position, 1.0)) + vec4(uni_model_pos, 0.0);
	vec4 view_pos = uni_view * model_pos;

	gl_Position = uni_proj * view_pos;
	//gl_Position = vec4(view_pos.x/10.0, view_pos.y/10.0, 0.5, 1.0);

	float drugs_y = (
				(cos(gl_Position.z + (uni_time * fac1)) / 2)
			) * (
				(sin(gl_Position.x + (uni_time * fac2)) / 2)
			) * fac3;

	gl_Position.y =
			gl_Position.y + drugs_y + (
				uni_curvature * (
					(
						gl_Position.w * gl_Position.w
					) + (
						gl_Position.x * gl_Position.x
					) - 1.0
				)
			);

	screen_position = gl_Position.xyz;

	vec3 pos = (transpose(uni_view) * view_pos).xyz;
	//vec3 pos = view_pos.xyz;
	vec3 lookat = normalize(pos);
	vec3 normal = normalize((uni_model * vec4(in_normal, 1.0)).xyz);
	float shade_amount = (dot(normal, uni_light_dir) * uni_shade) - uni_shade;
	float reflect_amount =
			uni_reflect *
			pow (
				max(
					0,
					dot(
						normalize(
							reflect(
								uni_light_dir,
								normal
							)
						),
						lookat
					)
				),
				uni_reflect_falloff
			);

	ex_color = in_color + vec4(vec3(shade_amount + reflect_amount), 1.0);
	//ex_color = vec4(vec3(reflect_amount), 1.0);
	//ex_color = vec4((lookat/2.0) + 1.0, 1.0);
}
