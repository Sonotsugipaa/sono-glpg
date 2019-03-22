#ifndef SONOGLPG_GLOBJECT_HPP
#define SONOGLPG_GLOBJECT_HPP

#include <vector>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>




namespace gla {

	/* Deleted member functions: copying a VertexBuffer or a
	 * VertexArray *WILL* trigger unexpected and duplicate
	 * deallocations. */

	class VertexBuffer {
	protected:
		GLenum target;
		GLuint vbo_id;
		GLenum usage;
	public:

		VertexBuffer(GLuint target, GLenum usage);
		VertexBuffer(const VertexBuffer &) = delete;
		VertexBuffer(VertexBuffer&&);
		~VertexBuffer();

		void bind();
		GLuint id();
		void bufferData(void* data_pointer, size_t data_size);

		VertexBuffer& operator = (const VertexBuffer &) = delete;
		VertexBuffer& operator = (VertexBuffer&&) = delete;
	};


	class VertexArray {
	protected:
		GLuint vao_id;
	public:
		VertexArray();
		VertexArray(const VertexArray &) = delete;
		VertexArray(VertexArray&&);
		~VertexArray();

		void assignVertexBuffer(
				VertexBuffer& vertex_buffer, GLuint attrib_pos,
				GLint size, GLenum datum_type, GLboolean normalized,
				GLsizei stride, GLvoid* offset );
		void bind();
		GLuint id();

		VertexArray& operator = (const VertexArray &) = delete;
		VertexArray& operator = (VertexArray&&) = delete;
	};

}

#endif
