#include "sneka/snekaruntime.hpp"
#include "sneka/shaders.hpp"

#pragma GCC warning "header needs const-correct, reentrant redesign"



namespace {
	bool shaderInit = false;
}


namespace sneka::shader {

	namespace level {

		unsigned int
			uniform_proj = -1,
			uniform_view = -1,
			uniform_view_pos = -1,
			uniform_model = -1,
			uniform_model_pos = -1,
			uniform_mul_col = -1,
			uniform_time = -1,
			uniform_curvature = -1,
			uniform_drugs = -1,
			uniform_fog = -1,

			uniform_light_color = -1,
			uniform_light_dir = -1,
			uniform_light_count = -1,

			uniform_shade = -1,
			uniform_reflect = -1,
			uniform_reflect_falloff = -1,
			uniform_reflect_opaque = -1,
			uniform_reflect_negative = -1,

			in_position = -1,
			in_color = -1,
			in_normal = -1;

	}

	void init(const SnekaRuntime & runtime) {
		using namespace shader::level;
		if(shaderInit) {
			throw SnekaRuntimeException(runtime, "shaders initialized twice");
		} else {
			gla::ShaderProgram& sh = runtime.getWorldShader();
			#define GET_UNI(NAME)     sh.getUniform(NAME);
			#define GET_ATTRIB(NAME)  sh.getAttrib(NAME);
				uniform_proj =              GET_UNI("uni_proj");
				uniform_view =              GET_UNI("uni_view");
				uniform_view_pos =          GET_UNI("uni_view_pos");
				uniform_model =             GET_UNI("uni_model");
				uniform_model =             GET_UNI("uni_model");
				uniform_model_pos =         GET_UNI("uni_model_pos");
				uniform_mul_col =           GET_UNI("uni_mul_col");
				uniform_time =              GET_UNI("uni_time");
				uniform_curvature =         GET_UNI("uni_curvature");
				uniform_drugs =             GET_UNI("uni_drugs");
				uniform_fog =               GET_UNI("uni_fog");

				uniform_light_color =       GET_UNI("uni_light_color");
				uniform_light_dir =         GET_UNI("uni_light_dir");
				uniform_light_count =       GET_UNI("uni_light_count");

				uniform_shade =             GET_UNI("uni_shade");
				uniform_reflect =           GET_UNI("uni_reflect");
				uniform_reflect_falloff =   GET_UNI("uni_reflect_falloff");
				uniform_reflect_opaque =    GET_UNI("uni_reflect_opaque");
				uniform_reflect_negative =  GET_UNI("uni_reflect_negative");

				in_position = GET_ATTRIB("in_position");
				in_color =    GET_ATTRIB("in_color");
				in_normal =   GET_ATTRIB("in_normal");
				//in_random =   GET_ATTRIB("in_random");
			#undef GET_UNI
			#undef GET_ATTRIB

			shaderInit = true;
		}
	}

}
