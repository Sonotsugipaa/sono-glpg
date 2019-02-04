#ifndef SNEKA_MESH_HPP
#define SNEKA_MESH_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <string>

#include "globject.hpp"

#define SNEKA_VERTEX_SIZE  (3+4)



namespace sneka {

	class Mesh {
	protected:
		gla::VertexBuffer vb;
		gla::VertexArray va;
		GLfloat * vertices_raw = nullptr;
		const GLsizei vertices_n;

	public:
		const std::string name;

		Mesh(
				std::string name,
				GLfloat* vertices_ptr, GLsizei vertices_count,
				bool keep = false);
		Mesh(Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		~Mesh();

		void draw() const;
		GLsizei size() const;
		const gla::VertexArray & getVertexArray() const;

		/* If the vertices were kept upon construction,
		 * free their data from the system memory without
	 	 * invalidating the VertexArray. */
		void forgetVertices();

		bool hasVertices();

		GLfloat operator [] (GLsizei);

		Mesh& operator = (Mesh&) = delete;
		Mesh& operator = (Mesh&&) = delete;
	};

}

#endif
