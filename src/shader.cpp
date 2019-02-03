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

	void printShaderLinkingError(GLuint shaderProgram) {
		int maxLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		gla::CompilationException ex = gla::CompilationException(shaderProgramInfoLog);
		delete[] shaderProgramInfoLog;
		throw ex;
	}

	void printShaderCompilationErrorInfo(GLuint shaderId, const char * shaderName) {
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

		gla::CompilationException ex = gla::CompilationException(shaderName, shaderInfoLog);
		delete[] shaderInfoLog;
		throw ex;
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

		GLint wasCompiled;
		glGetShaderiv(*target_shader, GL_COMPILE_STATUS, &wasCompiled);
		if(! wasCompiled) {
			const char * shader_name;
			switch(shader_type) {
				case GL_VERTEX_SHADER:
					shader_name = "Vertex";  break;
				case GL_FRAGMENT_SHADER:
					shader_name = "Fragment";  break;
				case GL_GEOMETRY_SHADER:
					shader_name = "Geometry";  break;
				default: shader_name = "Unknown(?)";
			}
			printShaderCompilationErrorInfo(
					*target_shader,
					shader_name );
			exit(EXIT_FAILURE);
		}
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

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if(! isLinked) {
			printShaderLinkingError(program);
			exit(EXIT_FAILURE);
		}
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


	CompilationException::CompilationException(
			const char * message
	) {
		msg =
				std::string("\033[1;91mShader linking error.\n") +
				message + "\033[m\n";
	}

	CompilationException::CompilationException(
			const char * sh, const char * message
	) {
		msg =
				std::string("\033[1;91m") +
				sh+" shader compilation error.\n" +
				message + "\033[m\n";
	}

	const char * CompilationException::what() const noexcept {
		return msg.c_str();
	}

}
