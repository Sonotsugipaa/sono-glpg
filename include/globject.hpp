#ifndef SONOGLPG_GLOBJECT_HPP
#define SONOGLPG_GLOBJECT_HPP

#include <vector>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>




namespace gla {

	class VertexBuffer {
	protected:
		GLenum target;
		GLuint vbo_id;
	public:
		const GLenum usage;

		VertexBuffer(GLuint target, GLenum usage);
		~VertexBuffer();

		void bind();
		GLuint id();
		void bufferData(void* data_pointer, size_t data_size);
	};


	class VertexArray {
	protected:
		GLuint vao_id;
	public:
		VertexArray();
		~VertexArray();

		void assignVertexBuffer(
				VertexBuffer& vertex_buffer, GLuint attrib_pos,
				GLint size, GLenum datum_type, GLboolean normalized,
				GLsizei stride, GLvoid* offset );
		void bind();
		GLuint id();
	};

}

#endif
