#include "sneka/snekaruntime.hpp"

#include "sneka/shaders.hpp"



namespace {

	gla::ObjectCounter<> runtime_instances_counter;


	const unsigned char * keyboard = nullptr;

	void update_keyboard() {
		SDL_PumpEvents();
	}

}


namespace sneka {

	#pragma GCC warning "temporary HUD shader initialization"
	SnekaRuntime::SnekaRuntime(
			std::string window_name,
			int x, int y,
			int width, int height,
			bool resizable,
			bool vsync
	):
			gla::Runtime(window_name, x, y, width, height, resizable, vsync),
			key_cb([](sneka::SnekaRuntime&, unsigned int, unsigned int, bool) { }),
			rsz_cb([](sneka::SnekaRuntime& rt, unsigned int x, unsigned int y) {
				rt.setViewport(x,y);
			}),
			vp_w(width), vp_h(height),
			vp_r(static_cast<GLfloat>(width) / static_cast<GLfloat>(height)),
			vp_s(0),
			runtime_instances(runtime_instances_counter),
			shader_world("shader/sneka_v.glsl", "shader/sneka_f.glsl"),
			shader_hud("shader/sneka_v.glsl", "shader/sneka_f.glsl")
	{
		shader::init(*this);
		glViewport(0, 0, width, height);
	}

	SnekaRuntime::~SnekaRuntime() { }


	GLuint SnekaRuntime::getViewportWidth() const {
		return vp_w;
	}

	GLuint SnekaRuntime::getViewportHeight() const {
		return vp_h;
	}

	GLfloat SnekaRuntime::getViewportRatio() const {
		return vp_r;
	}

	GLuint64 SnekaRuntime::getViewportState() const {
		return vp_w;
	}

	void SnekaRuntime::setViewport(GLuint width, GLuint height) {
		vp_w = width; vp_h = height;
		vp_r = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		vp_s += 1;
		glViewport(0, 0, width, height);
	}


	gla::ShaderProgram& SnekaRuntime::getWorldShader() const {
		return shader_world;
	}

	gla::ShaderProgram& SnekaRuntime::getHudShader() const {
		return shader_hud;
	}


	SnekaRuntimeException::SnekaRuntimeException(
			const SnekaRuntime & rt, const std::string & msg
	):
			runtime(rt), msg(msg)
	{ }

	SnekaRuntimeException::SnekaRuntimeException(
			const SnekaRuntime & rt, const char * msg
	):
			SnekaRuntimeException::SnekaRuntimeException(rt, std::string(msg))
	{ }

	const char * SnekaRuntimeException::what() const noexcept {
		return msg.c_str();
	}

}
