#include "sneka/floorobject.hpp"

#include "sneka/pool.hpp"
#include "sneka/mesh.hpp"



namespace {

	using namespace sneka;

	/*
	void print_all_vertices(Mesh& vertices) {
		GLsizei sz = vertices.size();

		for(GLsizei i=0; i<sz; i+=1) {
			std::cout << vertices[(i * SNEKA_VERTEX_SIZE)];
			for(GLsizei j=1; j < SNEKA_VERTEX_SIZE; j+=1) {
				std::cout << ", " << vertices[(i * SNEKA_VERTEX_SIZE) + j];
			}
			std::cout << std::endl;
		}
	}
	*/

	void expand_floor(
			std::string name,
			Mesh* tile_mesh, GLfloat tile_size,
			Mesh** dest, GLsizei floor_size_length
	) {
		GLsizei area = floor_size_length * floor_size_length;
		GLfloat* new_array = new GLfloat[area * tile_mesh->size() * SNEKA_VERTEX_SIZE];
		GLfloat* new_array_cur = new_array;

		for(GLint x=0; x < floor_size_length; x += 1) {
			for(GLint z=0; z < floor_size_length; z += 1) {
				new_array_cur = new_array + ((x + (z * floor_size_length)) * SNEKA_VERTEX_SIZE * tile_mesh->size());

				for(int i=0; i < tile_mesh->size(); i += 1) {
					int at = i * SNEKA_VERTEX_SIZE;
					new_array_cur[at+0] = (*tile_mesh)[at+0] + x;
					new_array_cur[at+1] = (*tile_mesh)[at+1];
					new_array_cur[at+2] = (*tile_mesh)[at+2] + z;

					for(GLsizei j=3; j < SNEKA_VERTEX_SIZE; j += 1) {
						new_array_cur[at+j] = (*tile_mesh)[at+j];
					}
				}
			}
		}

		*dest = new Mesh(
				name+":"+std::to_string(floor_size_length),
				new_array,
				area * tile_mesh->size() );
		delete[] new_array;
	}

}


namespace sneka {

	FloorObject::FloorObject(
			const std::string & tile_mesh,
			int unsigned side_length
	):
			side_length(side_length)
	{
		Mesh* nmesh;
		expand_floor(
				tile_mesh,
				&pool::get_mesh(tile_mesh), 1.0f,
				&nmesh,
				side_length );
		RenderObject::mesh = nmesh;
	}

	FloorObject::~FloorObject() {
		delete RenderObject::mesh;
	}

}
