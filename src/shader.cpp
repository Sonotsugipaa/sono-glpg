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
		std::cout << "\033[1;91mShader linking failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::cout << "Info Length : " << maxLength << std::endl;

		// Get shader info log
		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		std::cout
				<< "Linker error message : " << shaderProgramInfoLog
		 		<< "\033[m" << std::endl;

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;
		return;
	}

	void printShaderCompilationErrorInfo(GLuint shaderId) {
		std::cout << "\033[1;91mShader compilation failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// Get shader info log
		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

		// Print shader info log
		std::cout
			<< "\tError info : " << shaderInfoLog << "\033[m" << std::endl;
		delete shaderInfoLog;
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
			printShaderCompilationErrorInfo(*target_shader);
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

}
