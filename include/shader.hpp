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
