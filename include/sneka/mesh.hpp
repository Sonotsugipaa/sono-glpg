#ifndef SNEKA_MESH_HPP
#define SNEKA_MESH_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <string>

#include "globject.hpp"

#include "sneka/asset.hpp"

#define SNEKA_VERTEX_SIZE  (3+4)



namespace sneka {

	class Mesh : public virtual Asset {
	protected:
		gla::VertexBuffer vb, vb_normal;
		gla::VertexArray va;
		GLfloat * vertices_raw;
		GLsizei vertices_n;

	public:
		Mesh(
				std::string name,
				GLfloat* vertices_ptr, GLsizei vertices_count,
				bool keep = false );

		Mesh(Mesh&) = delete;
		Mesh(Mesh&&);
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


	class MeshLoader : public AssetLoader<Mesh> {
		friend Mesh;
	protected:
		virtual Mesh* load(std::string mesh_name);

	public:
		virtual void cleanup();
	};

}

#endif
