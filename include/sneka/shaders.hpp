#include "sneka/SnekaRuntime.hpp"
// Definitions must be in the shader's respective renderer

namespace sneka::shader {

	namespace level {

		extern unsigned int
			uniform_proj,
			uniform_view,
			uniform_view_pos,
			uniform_model,
			uniform_model_pos,
			uniform_mul_col,
			uniform_time,
			uniform_curvature,
			uniform_drugs,
			uniform_fog,

			uniform_light_color,
			uniform_light_dir,
			uniform_light_count,

			uniform_shade,
			uniform_reflect,
			uniform_reflect_falloff,
			uniform_reflect_opaque,
			uniform_reflect_negative,

			in_position,
			in_color,
			in_normal;

	}


	void init(const SnekaRuntime & runtime);

}
