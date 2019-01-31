#ifndef SONOGLPG_SHADER_HPP
#define SONOGLPG_SHADER_HPP

#include <string>



namespace gla {

	class ShaderProgram {
	public:
		GLuint vertex, fragment, program;

		ShaderProgram(std::string vertex, std::string fragment);
		~ShaderProgram();

		void use();
	};

}

#endif
