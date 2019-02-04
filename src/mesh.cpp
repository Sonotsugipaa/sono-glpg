#include "sneka/mesh.hpp"

/* do NOT move this inclusion to sneka/mesh.hpp, that
 * would create a circular dependency (which is sort of
 * already happening, soooo...) */
#include "sneka/pool.hpp"

#include <stdexcept>



namespace {

	void invalid_access() {
		throw std::logic_error("Tried to access forgotten or non-existent data from a mesh.");
	}

}


namespace sneka {

	using namespace gla;

	/*
	Mesh::Mesh(const char * nm, GLfloat* vertices, GLsizei n):
			Mesh::Mesh(std::string(nm), vertices, n)
	{ }
	*/

	Mesh::Mesh(std::string nm, GLfloat* vertices, GLsizei n, bool keep):

			vb(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
			vertices_n(n), name(nm)
	{
		if(keep) {
			vertices_raw = new GLfloat[n * SNEKA_VERTEX_SIZE];
			GLsizei sz = n * SNEKA_VERTEX_SIZE;
			for(GLsizei i=0; i < sz; i+=1)
				vertices_raw[i] = vertices[i];
		}

		vb.bufferData(vertices, n * SNEKA_VERTEX_SIZE * sizeof(GLfloat));

		va.assignVertexBuffer(
				vb, pool::in_position,
				3, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (0 * sizeof(GLfloat)) );
		va.assignVertexBuffer(
				vb, pool::in_color,
				4, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (3 * sizeof(GLfloat)) );
	}

	Mesh::~Mesh() {
		if(vertices_raw != nullptr) {
			delete[] vertices_raw;
		}
	}


	void Mesh::forgetVertices() {
		if(vertices_raw == nullptr)
			return;

		delete[] vertices_raw;
		vertices_raw = nullptr;
	}

	bool Mesh::hasVertices() {
		return vertices_raw != nullptr;
	}

	GLfloat Mesh::operator [] (GLsizei i) {
		if(vertices_raw == nullptr)
			invalid_access();
		return vertices_raw[i];
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
