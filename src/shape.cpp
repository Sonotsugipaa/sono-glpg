#define SONOGLPG_SHAPE_HPP_NOWARN
#include "shapes3d.hpp"



namespace gla {

	Shape::Shape(GLenum mode, VertexBuffer& vb_vertex, VertexBuffer& vb_color, std::size_t vectors_n):
			draw_mode(mode), vectors_n(vectors_n)
	{
		va.assignVertexBuffer(
				vb_vertex, 0,
				3, GL_FLOAT, GL_FALSE,
				0, 0 );
		va.assignVertexBuffer(
				vb_color, 1,
				4, GL_FLOAT, GL_FALSE,
				0, 0 );
	}

	void Shape::draw() {
		va.bind();
		glDrawArrays(draw_mode, 0, vectors_n);
	}

}
