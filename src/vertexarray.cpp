#include "globject.hpp"



namespace gla {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &vao_id);
	}

	VertexArray::VertexArray(VertexArray&& mov):
			vao_id(mov.vao_id)
	{
		mov.vao_id = 0;
	}

	VertexArray::~VertexArray() {
		if(vao_id != 0)
			glDeleteVertexArrays(1, &vao_id);
	}


	void VertexArray::bind() {
		glBindVertexArray(vao_id);
	}

	void VertexArray::assignVertexBuffer(
			VertexBuffer& vb, GLuint attrib_pos, GLint size,
			GLenum datum_type, GLboolean normalized,
			GLsizei stride, GLvoid* offset
	) {
		bind();
		vb.bind();
		glVertexAttribPointer(
				attrib_pos,
				size,       /* number of values */
				datum_type, /* type of values   */
				normalized, /* normalized       */
				stride,     /* stride           */
				offset      /* offset           */ );
		glEnableVertexAttribArray(attrib_pos);
	}

	GLuint VertexArray::id() {
		return vao_id;
	}

}
