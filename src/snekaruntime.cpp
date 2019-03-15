#include "sneka/snekaruntime.hpp"

#include "sneka/shaders.hpp"

#include <map>

#include <cstdio>



namespace {

	std::map<std::string, sneka::Mesh*> pool_meshes;
	gla::ObjectCounter<> runtime_instances_counter;


	void read_int_to(FILE* file, GLfloat* dest) {
		int i=0;
		fread(&i, sizeof(int), 1, file);
		*dest = (GLfloat) i / POOL_MESH_FILE_PRECISION;
	}

	sneka::Mesh* load_mesh(const sneka::SnekaRuntime & rt, std::string path, bool need_vertices) {
		GLfloat* vertices;
		GLsizei len;

		FILE* f = fopen(path.c_str(), "rb");
		if(! f)
			throw sneka::SnekaRuntimeException(rt, "couldn't open \"" + path + "\".");

		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		if(len % (sizeof(int) * SNEKA_VERTEX_SIZE) != 0) {
			throw sneka::SnekaRuntimeException(
					rt,
					"\"" + path + "\": invalid file size (" +
					std::to_string(len) + ")" );
		}
		len /= (sizeof(int) * SNEKA_VERTEX_SIZE);

		vertices = new GLfloat[len * SNEKA_VERTEX_SIZE];

		for(GLsizei i=0; i<len; i+=1) {
			for(GLsizei j=0; j < SNEKA_VERTEX_SIZE; j+=1)
				read_int_to(f, vertices + j + (i * SNEKA_VERTEX_SIZE));
		}
		fclose(f);

		sneka::Mesh* retn = new sneka::Mesh(path, vertices, len, need_vertices);
		delete[] vertices;
		return retn;
	}

	void unload_meshes() {
		auto iter = pool_meshes.begin();
		auto end = pool_meshes.end();

		while(iter != end) {
			//std::cout << "Unloaded mesh " << iter->second->name << '.' << std::endl;
			delete iter->second;
			pool_meshes.erase(iter);
			iter = pool_meshes.begin();
		}
	}

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

	SnekaRuntime::~SnekaRuntime() {
		if(runtime_instances.value() == 1)
			unload_meshes();
	}


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


	Mesh& SnekaRuntime::getMesh(std::string name, bool need_vertices) const {
		std::map<std::string, Mesh*>::iterator iter;

		iter = pool_meshes.find(name);

		if(iter != pool_meshes.end() && need_vertices) {
			if(! iter->second->hasVertices()) {
				delete iter->second;
				pool_meshes.erase(iter);
				iter = pool_meshes.end();
			}
		}

		if(iter == pool_meshes.end()) {

			pool_meshes[name] = load_mesh(*this, name, need_vertices);

			iter = pool_meshes.find(name);
			if(iter == pool_meshes.end()) {
				throw SnekaRuntimeException(*this, "could not load mesh \""+name+"\"");
			}
		}

		return *(iter->second);
	}


	SnekaRuntimeException::SnekaRuntimeException(
			const SnekaRuntime & rt, const std::string & msg
	):
			runtime(rt), msg(msg)
	{ }

	SnekaRuntimeException::SnekaRuntimeException(
			const SnekaRuntime & rt, const std::string && msg
	):
			SnekaRuntimeException::SnekaRuntimeException(rt, msg)
	{ }

	SnekaRuntimeException::SnekaRuntimeException(
			const SnekaRuntime & rt, char* msg
	):
			SnekaRuntimeException::SnekaRuntimeException(rt, std::string(msg))
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
