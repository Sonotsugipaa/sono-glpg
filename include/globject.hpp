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
		const GLenum target;
		GLuint vbo_id;
	public:
		const GLenum usage;

		VertexBuffer(GLuint target, GLenum usage);
		VertexBuffer(VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;
		~VertexBuffer();

		void bind() const;
		GLuint id() const;
		void bufferData(void* data_pointer, GLsizei data_size);

		VertexBuffer& operator = (VertexBuffer&) = delete;
		VertexBuffer& operator = (VertexBuffer&&) = delete;
	};


	class VertexArray {
	protected:
		GLuint vao_id;
	public:
		VertexArray();
		VertexArray(VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;
		~VertexArray();

		void assignVertexBuffer(
				VertexBuffer& vertex_buffer, GLuint attrib_pos,
				GLint size, GLenum datum_type, GLboolean normalized,
				GLsizei stride, GLvoid* offset );
		void bind() const;
		GLuint id() const;

		VertexArray& operator = (VertexArray&) = delete;
		VertexArray& operator = (VertexArray&&) = delete;
	};

}

#endif
