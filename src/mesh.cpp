#include "sneka/mesh.hpp"

/* do NOT move this inclusion to sneka/mesh.hpp, that
 * would create a circular dependency (which is sort of
 * already happening, soooo...) */
#include "sneka/pool.hpp"

#include <iostream> // Debug only



namespace sneka {

	using namespace gla;

	/*
	Mesh::Mesh(const char * nm, GLfloat* vertices, GLsizei n):
			Mesh::Mesh(std::string(nm), vertices, n)
	{ }
	*/

	Mesh::Mesh(std::string nm, GLfloat* vertices, GLsizei n):

			vb(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
			vertices_n(n), name(nm)
	{
		std::cout << n << std::endl;
		vb.bufferData(vertices, n * SNEKA_VERTEX_SIZE * sizeof(GLfloat));

		va.assignVertexBuffer(
				vb, pool::in_position,
				3, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (0 * sizeof(GLfloat)) );
		va.assignVertexBuffer(
				vb, pool::in_color,
				4, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (3 * sizeof(GLfloat)) );
		/*va.assignVertexBuffer(
				vb, pool::in_random,
				1, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (7 * sizeof(GLfloat)) );*/
	}

	void Mesh::draw() const {
		va.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices_n);
	}

	GLsizei Mesh::size() const {
		return vertices_n;
	}

	const VertexArray & Mesh::getVertexArray() const {
		return const_cast<const VertexArray &>(va);
	}

}
