#ifndef SONOGLPG_SHADER_HPP
#define SONOGLPG_SHADER_HPP

#include <string>
#include <exception>



namespace gla {

	class ShaderProgram {
	private:
		GLuint vertex, fragment, program;
	public:

		ShaderProgram(std::string vertex, std::string fragment);
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		~ShaderProgram();

		constexpr GLuint getVertexId() const { return vertex; }
		constexpr GLuint getFragmentId() const { return fragment; }
		constexpr GLuint getId() const { return program; }

		GLint getAttrib(const char * name) const;
		GLint getAttrib(const std::string & name) const;
		GLint getUniform(const char * name) const;
		GLint getUniform(const std::string & name) const;

		void use();

		ShaderProgram& operator = (ShaderProgram&) = delete;
		ShaderProgram& operator = (ShaderProgram&&) = delete;
	};

	class CompilationException : public std::exception {
	private:
		std::string msg;
	public:
		 // Linking error
		CompilationException(const char * message);
		 // Compilation error
		CompilationException(const char * shader, const char * message);

		const char * what() const noexcept;
	};

}

#endif
