#version 130

uniform mat4 uni_model;
uniform mat4 uni_view;
uniform mat4 uni_proj;
uniform vec3 uni_model_pos;
uniform vec3 uni_view_pos;
uniform float uni_shade;
uniform float uni_reflect;
uniform float uni_reflect_falloff;
uniform float uni_reflect_opaque;
uniform float uni_reflect_negative;
uniform float uni_time;
uniform float uni_curvature;
uniform float uni_drugs;

uniform vec3 uni_light_color;
uniform vec3 uni_light_dir[8];
uniform int uni_light_count;

in vec3 in_position;
in vec4 in_color;
in vec3 in_normal;

out vec4 ex_color;
out vec3 ex_reflect;
out vec3 ex_absorb;
out vec3 screen_position;

float falloff(float x) {
	return ((uni_reflect_falloff * x) + x) - uni_reflect_falloff;
}

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

	float curv_y = uni_curvature * (
		(
			gl_Position.w * gl_Position.w
		) + (
			gl_Position.x * gl_Position.x
		) - 1.0
	);

	gl_Position.y = gl_Position.y + drugs_y + curv_y;

	screen_position = gl_Position.xyz;

	vec3 pos = (transpose(uni_view) * view_pos).xyz;
	vec3 lookat = normalize(pos);
	vec3 normal = normalize((uni_model * vec4(in_normal, 1.0)).xyz);
	float shade_amount = 0;
	vec3 reflect_amount = vec3(0.0);
	for(int i=0; i < uni_light_count; i+=1) {
		shade_amount +=
				(
					max(
						0,
						dot(
							normal,
							uni_light_dir[i]
						) * uni_reflect_opaque
					)
				);
		reflect_amount +=
				uni_light_color * (
					uni_reflect *
					max(
						uni_reflect_negative,
						falloff(
							dot(
								normalize(
									reflect(
										uni_light_dir[i],
										normal
									)
								),
								lookat
							)
						)
					)
				);
	}
	shade_amount = (shade_amount / uni_light_count) - uni_shade;

	/* Is using 3 times the max function better than if-branching?
	 * The internet says so. */
	ex_color = in_color;
	ex_color.r = max(0, ex_color.r);
	ex_color.g = max(0, ex_color.g);
	ex_color.b = max(0, ex_color.b);

	ex_absorb = in_color.xyz;
	ex_absorb.r = min(0, ex_absorb.r);
	ex_absorb.g = min(0, ex_absorb.g);
	ex_absorb.b = min(0, ex_absorb.b);

	ex_reflect = reflect_amount + vec3(shade_amount);
}
