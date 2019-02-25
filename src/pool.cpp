#include "sneka/pool.hpp"

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

		sneka::Mesh* retn = new sneka::Mesh(path, vertices, len, true);
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

	GLuint
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

		uniform_light_dir = -1,
		uniform_light_count = -1,

		uniform_shade = -1,
		uniform_reflect = -1,
		uniform_reflect_falloff = -1,
		uniform_reflect_opaque = -1,

		in_position = -1,
		in_color = -1,
		in_normal = -1;


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
				resizable, vsync,
				"shader/sneka_v.glsl", "shader/sneka_f.glsl" );

		set_viewport(width, height);

		uniform_proj =            glGetUniformLocation(runtime_inst->shader->program, "uni_proj");
		uniform_view =            glGetUniformLocation(runtime_inst->shader->program, "uni_view");
		uniform_view_pos =        glGetUniformLocation(runtime_inst->shader->program, "uni_view_pos");
		uniform_model =           glGetUniformLocation(runtime_inst->shader->program, "uni_model");
		uniform_model =           glGetUniformLocation(runtime_inst->shader->program, "uni_model");
		uniform_model_pos =       glGetUniformLocation(runtime_inst->shader->program, "uni_model_pos");
		uniform_mul_col =         glGetUniformLocation(runtime_inst->shader->program, "uni_mul_col");
		uniform_time =            glGetUniformLocation(runtime_inst->shader->program, "uni_time");
		uniform_curvature =       glGetUniformLocation(runtime_inst->shader->program, "uni_curvature");
		uniform_drugs =           glGetUniformLocation(runtime_inst->shader->program, "uni_drugs");
		uniform_fog =             glGetUniformLocation(runtime_inst->shader->program, "uni_fog");

		uniform_light_dir =       glGetUniformLocation(runtime_inst->shader->program, "uni_light_dir");
		uniform_light_count =     glGetUniformLocation(runtime_inst->shader->program, "uni_light_count");

		uniform_shade =           glGetUniformLocation(runtime_inst->shader->program, "uni_shade");
		uniform_reflect =         glGetUniformLocation(runtime_inst->shader->program, "uni_reflect");
		uniform_reflect_falloff = glGetUniformLocation(runtime_inst->shader->program, "uni_reflect_falloff");
		uniform_reflect_opaque =  glGetUniformLocation(runtime_inst->shader->program, "uni_reflect_opaque");

		in_position = glGetAttribLocation(runtime_inst->shader->program, "in_position");
		in_color =    glGetAttribLocation(runtime_inst->shader->program, "in_color");
		in_normal =   glGetAttribLocation(runtime_inst->shader->program, "in_normal");
		//in_random =   glGetAttribLocation(runtime_inst->shader->program, "in_random");
	}

	void runtime_destroy() {
		assert_runtime_init(true);

		unload_meshes();

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
