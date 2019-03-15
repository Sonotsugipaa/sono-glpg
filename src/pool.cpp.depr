#include "sneka/pool.hpp"

#include "sneka/shaders.hpp"

#include <map>

#include <iostream> // Debug only

#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace {

	gla::Runtime* runtime_inst = nullptr;

	std::map<std::string, sneka::Mesh*> pool_meshes;

	glm::mat4 world_proj_matrix_inst = glm::mat4(1.0f);

	GLuint vp_w, vp_h;
	GLuint64 vp_change = 0;
	GLfloat vp_r;

	gla::ShaderProgram * shader_world;
	gla::ShaderProgram * shader_hud;


	void assert_runtime_init(bool init_state) {
		if((init_state) && (runtime_inst == nullptr))
			throw sneka::pool::PoolException("Uninitialized runtime.");

		if((! init_state) && (runtime_inst != nullptr))
			throw sneka::pool::PoolException("Runtime already initialized.");
	}

	void read_int_to(FILE* file, GLfloat* dest) {
		int i=0;
		fread(&i, sizeof(int), 1, file);
		*dest = (GLfloat) i / POOL_MESH_FILE_PRECISION;
	}

	sneka::Mesh* load_mesh(std::string path, bool need_vertices) {
		GLfloat* vertices;
		GLsizei len;

		FILE* f = fopen(path.c_str(), "rb");
		if(! f)
			throw sneka::pool::PoolException("couldn't open \"" + path + "\".");

		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		if(len % (sizeof(int) * SNEKA_VERTEX_SIZE) != 0) {
			throw sneka::pool::PoolException(
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

}


namespace sneka::pool {

	using namespace gla;


	const Runtime * runtime() {
		assert_runtime_init(true);
		return const_cast<const Runtime *>(runtime_inst);
	}

	void runtime_init(
			std::string window_name,
			int x, int y,
			int width, int height,
			bool resizable,
			bool vsync
	) {
		assert_runtime_init(false);

		runtime_inst = new Runtime(
				window_name,
				x, y, width, height,
				resizable, vsync );

		set_viewport(width, height);

		#pragma GCC warning "Temporary HUD shader initialization"
		shader_world = new ShaderProgram("shader/sneka_v.glsl", "shader/sneka_f.glsl");
		shader_hud = new ShaderProgram("shader/sneka_v.glsl", "shader/sneka_f.glsl");

		shader::init();
	}

	void runtime_destroy() {
		assert_runtime_init(true);

		unload_meshes();

		delete shader_world;
		delete shader_hud;
		delete runtime_inst;
		runtime_inst = nullptr;

		SDL_Quit();
	}

	GLuint viewport_width()  { return vp_w; }
	GLuint viewport_height() { return vp_h; }
	GLfloat viewport_ratio() { return vp_r; }

	void set_viewport(GLuint w, GLuint h) {
		vp_w = w; vp_h = h;
		vp_r = static_cast<GLfloat>(w) / static_cast<GLfloat>(h);
		vp_change += 1;
		glViewport(0, 0, w, h);
	}

	GLuint64 viewport_change() { return vp_change; }

	ShaderProgram& getWorldShader() { return *shader_world; }
	ShaderProgram& getHudShader() { return *shader_hud; }

	Mesh& get_mesh(std::string name, bool need_vertices) {
		assert_runtime_init(true);

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

			pool_meshes[name] = load_mesh(name, need_vertices);

			iter = pool_meshes.find(name);
			if(iter == pool_meshes.end()) {
				throw PoolException("could not load mesh \""+name+"\"");
			}
		}

		return *(iter->second);
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
