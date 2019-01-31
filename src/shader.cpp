#include <sstream>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "shader.hpp"



namespace {

	std::string get_source(std::string& file) {
		std::ifstream file_str(file);
		std::stringstream buffer;

		buffer << file_str.rdbuf();

		return buffer.str();
	}

	void compile_shader(
			GLuint* target_shader,
			std::string file,
			GLenum shader_type
	) {
		std::string src = get_source(file);
		GLint size = src.size();
		const char * csrc = const_cast<char*>(src.c_str());

		*target_shader = glCreateShader(shader_type);
		glShaderSource(*target_shader, 1, &csrc, &size);
		glCompileShader(*target_shader);
	}

}


namespace gla {

#include <iostream>
	ShaderProgram::ShaderProgram(std::string v, std::string f) {
		compile_shader(&vertex,   v, GL_VERTEX_SHADER);
		compile_shader(&fragment, f, GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
	}

	ShaderProgram::~ShaderProgram() {
		glUseProgram(0);
		glDetachShader(program, vertex);
		glDetachShader(program, fragment);

		glDeleteProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void ShaderProgram::use() {
		glUseProgram(program);
	}

}
