#ifndef SONOGLPG_SHADER_HPP
#define SONOGLPG_SHADER_HPP

#include <string>
#include <exception>



namespace gla {

	class ShaderProgram {
	public:
		GLuint vertex, fragment, program;

		ShaderProgram(std::string vertex, std::string fragment);
		~ShaderProgram();

		void use();
	};

	class CompilationException : public std::exception {
		std::string msg;
	public:
		CompilationException(bool linker, char* message);
		const char * what() const noexcept;
	};

}

#endif
