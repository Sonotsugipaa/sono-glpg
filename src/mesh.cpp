#include "sneka/mesh.hpp"

/* do NOT move this inclusion to sneka/mesh.hpp, that
 * would create a circular dependency (which is sort of
 * already happening, soooo...) */
//#include "sneka/pool.hpp"

#include "sneka/shaders.hpp"

#include <stdexcept>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <cstdio>



namespace {

	void invalid_access() {
		throw std::logic_error("Tried to access forgotten or non-existent data from a mesh.");
	}

	/* only works for GL_TRIANGLE meshes, don't try this on GL_TRIANGLE_FAN
	 * or anything else */
	void tri_normals(GLfloat* dest, GLfloat* vertices, GLsizei n) {
		using namespace glm;

		/* number of vertices  -->  number of triangles */
		n /= 3;

		// for each triangle
		for(GLsizei i = 0; i < n; i += 1) {
			// each vertex of the triangle
			GLsizei i1 = i * (3 * SNEKA_VERTEX_SIZE);
			GLsizei i2 = i1 + SNEKA_VERTEX_SIZE;
			GLsizei i3 = i2 + SNEKA_VERTEX_SIZE;

			// position vector of each vertex
			vec3 v1 = vec3(vertices[i1], vertices[i1+1], vertices[i1+2]);
			vec3 v2 = vec3(vertices[i2], vertices[i2+1], vertices[i2+2]);
			vec3 v3 = vec3(vertices[i3], vertices[i3+1], vertices[i3+2]);

			vec3 U = v2 - v1;
			vec3 V = v3 - v1;

			vec3 normal = normalize(vec3(
				(U[1] * V[2]) - (U[2] * V[1]),
				(U[2] * V[0]) - (U[0] * V[2]),
				(U[0] * V[1]) - (U[1] * V[0])
			));

			// for each normal
			for(GLsizei j=0; j<3; j+=1) {
				GLsizei at = 3 * ((3*i)+j);

				// for each normal coordinate
				for(GLsizei k=0; k<3; k+=1) {
					dest[at+k] = normal[k];
				}
			}
		}
	}

	// used for MeshLoader::load(...)
	inline void read_int_to(FILE* file, GLfloat* dest) {
		int i=0;
		fread(&i, sizeof(int), 1, file);
		*dest = (GLfloat) i / POOL_MESH_FILE_PRECISION;
	}

}


namespace sneka {

	using namespace gla;


	// -----  MeshLoader  -----


	Mesh::Mesh(std::string nm, GLfloat* vertices, GLsizei n, bool keep):
			Asset::Asset(nm),
			vb(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
			vb_normal(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
			vertices_raw(nullptr),
			vertices_n(n)
	{
		if(keep) {
			vertices_raw = new GLfloat[n * SNEKA_VERTEX_SIZE];
			GLsizei sz = n * SNEKA_VERTEX_SIZE;
			for(GLsizei i=0; i < sz; i+=1)
				vertices_raw[i] = vertices[i];
		}

		GLfloat* normals = new GLfloat[n * 3];
		tri_normals(normals, vertices, n);

		vb.bufferData(vertices, n * SNEKA_VERTEX_SIZE * sizeof(GLfloat));
		vb_normal.bufferData(normals, n * 3 * sizeof(GLfloat));

		va.assignVertexBuffer(
				vb, shader::level::in_position,
				3, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (0 * sizeof(GLfloat)) );
		va.assignVertexBuffer(
				vb, shader::level::in_color,
				4, GL_FLOAT, GL_FALSE,
				SNEKA_VERTEX_SIZE * sizeof(GLfloat), (GLfloat*) (3 * sizeof(GLfloat)) );

		va.assignVertexBuffer(
				vb_normal, shader::level::in_normal,
				3, GL_FLOAT, GL_FALSE,
				3 * sizeof(GLfloat), 0 );

		delete[] normals;
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


	// -----  MeshLoader  -----


	/* TODO: vertices are always remembered by the mesh,
	 * even though that is a waste of memory 99% of the time.
	 * This should be optimized, perhaps with a cleanup function. */
	Mesh* MeshLoader::load(std::string path) {
		GLfloat* vertices;
		GLsizei len;

		FILE* f = fopen(path.c_str(), "rb");
		if(! f)
			throw sneka::AssetLoadException(path, "couldn't open the file.");

		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		if(len % (sizeof(int) * SNEKA_VERTEX_SIZE) != 0) {
			throw sneka::AssetLoadException(
					path,
					"invalid file size (" + std::to_string(len) + ")" );
		}
		len /= (sizeof(int) * SNEKA_VERTEX_SIZE);

		vertices = new GLfloat[len * SNEKA_VERTEX_SIZE];

		for(GLsizei i=0; i<len; i+=1) {
			for(GLsizei j=0; j < SNEKA_VERTEX_SIZE; j+=1)
				read_int_to(f, vertices + j + (i * SNEKA_VERTEX_SIZE));
		}
		fclose(f);

		sneka::Mesh* retn = new sneka::Mesh(path, vertices, len, true);
		delete[] vertices;
		return retn;
	}

	void MeshLoader::cleanup() {
		auto iter = assets.begin();

		while(iter != assets.end()) {
			iter->second->forgetVertices();

			iter++;
		}
	}

}
