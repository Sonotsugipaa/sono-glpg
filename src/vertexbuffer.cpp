#include "globject.hpp"



namespace gla {

	VertexBuffer::VertexBuffer(GLuint target, GLenum usage):
			target(target), vbo_id(0), usage(usage)
	{
		glGenBuffers(1, &vbo_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& mov):
			target(mov.target), vbo_id(mov.vbo_id), usage(mov.usage)
	{
		mov.vbo_id = 0;
	}

	VertexBuffer::~VertexBuffer() {
		if(vbo_id != 0)
			glDeleteBuffers(1, &vbo_id);
	}


	void VertexBuffer::bind() {
		glBindBuffer(target, vbo_id);
	}

	GLuint VertexBuffer::id() {
		return vbo_id;
	}

	void VertexBuffer::bufferData(void* data_pointer, size_t data_size) {
		glBindBuffer(target, vbo_id);
		glBufferData(
				target,
				data_size,
				data_pointer,
				usage );
	}

}
