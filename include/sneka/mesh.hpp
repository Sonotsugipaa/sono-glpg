#ifndef SNEKA_MESH_HPP
#define SNEKA_MESH_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <string>

#include "globject.hpp"

#define SNEKA_VERTEX_SIZE  (3+4)



namespace sneka {

	class Mesh {
		gla::VertexBuffer vb;
		gla::VertexArray va;
		const GLsizei vertices_n;
	public:
		const std::string name;

		Mesh(std::string name, GLfloat* vertices_ptr, GLsizei vertices_count);
		//Mesh(const char * name, GLfloat* vertices_ptr, GLsizei vertices_count);
		Mesh(Mesh&) = delete;
		Mesh(Mesh&&) = delete;

		void draw() const;
		GLsizei size() const;
		const gla::VertexArray & getVertexArray() const;

		Mesh& operator = (Mesh&) = delete;
		Mesh& operator = (Mesh&&) = delete;
	};

}

#endif
