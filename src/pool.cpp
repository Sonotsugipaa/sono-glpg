#include "sneka/pool.hpp"



namespace {
	gla::Runtime* runtime_inst = nullptr;
}


namespace sneka::pool {

	using namespace gla;

	GLuint
		uniform_proj = -1,
		uniform_trans = -1,
		uniform_add_col = -1,
		uniform_time = -1,
		uniform_curvature = -1,
		uniform_drugs = -1;


	const Runtime * runtime() {
		if(runtime_inst == nullptr)
			throw PoolException("Uninitialized runtime.");

		return const_cast<const Runtime *>(runtime_inst);
	}

	void runtime_init(
			std::string window_name,
			int x, int y,
			int width, int height,
			bool resizable,
			bool vsync
	) {
		if(runtime_inst != nullptr)
			throw PoolException("Tried to reinitialize the current runtime.");

		runtime_inst = new Runtime(
				window_name,
				x, y, width, height,
				resizable, vsync,
				"shader/sneka_v.glsl", "shader/sneka_f.glsl" );

		uniform_proj =      glGetUniformLocation(runtime_inst->shader->program, "uni_proj");
		uniform_trans =     glGetUniformLocation(runtime_inst->shader->program, "uni_trans");
		uniform_add_col =   glGetUniformLocation(runtime_inst->shader->program, "uni_add_col");
		uniform_time =      glGetUniformLocation(runtime_inst->shader->program, "uni_time");
		uniform_curvature = glGetUniformLocation(runtime_inst->shader->program, "uni_curvature");
		uniform_drugs =     glGetUniformLocation(runtime_inst->shader->program, "uni_drugs");
	}

	void runtime_destroy() {
		if(runtime_inst == nullptr)
			throw PoolException("Uninitialized runtime.");

		delete runtime_inst;
		runtime_inst = nullptr;
	}


	PoolException::PoolException(std::string& msg):
			msg(msg)
	{ }

	PoolException::PoolException(std::string&& msg):
			PoolException::PoolException(msg)
	{ }

	PoolException::PoolException(char* msg):
			PoolException::PoolException(std::string(msg))
	{ }

	PoolException::PoolException(const char * msg):
			PoolException::PoolException(std::string(msg))
	{ }

	const char * PoolException::what() const noexcept {
		return msg.c_str();
	}

}
