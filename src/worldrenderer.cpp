#include "sneka/world.hpp"

#include "sneka/renderobject.hpp"
#include "sneka/mesh.hpp"
#include "sneka/pool.hpp"

#include "utils.tpp"

#include <iostream> // Debug only

#include <glm/gtc/matrix_transform.hpp>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>



namespace {

	glm::mat4 get_view(glm::vec3 pos, GLfloat yaw, GLfloat pitch) {
		glm::mat4 retn = glm::mat4(1.0f);

		// pitch
		retn = glm::rotate(
				retn,
				pitch,
				glm::vec3(1.0f, 0.0f, 0.0f) );

		// yaw
		retn = glm::rotate(
				retn,
				yaw,
				glm::vec3(0.0f, 1.0f, 0.0f) );

		return glm::translate(retn, pos);
	}

}


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

		*dest = new Mesh("?", new_array, area * tile_mesh->size());
		delete[] new_array;
	}

}


namespace sneka {

	WorldRenderer::WorldRenderer(
			std::string tile_mesh_name,
			GLuint tiles, GLfloat curv, GLfloat drugs,
			GLuint scr_w, GLuint scr_h
	):
			floor_tiles(tiles), floor_tiles_half(floor_tiles / 2.0f),
			curvature(curv), drugs(drugs)
	{
		mat_proj = glm::perspective(
				90.0f, (GLfloat) scr_w / scr_h,
				(GLfloat) WORLD_MIN_Z,
				(GLfloat) WORLD_MAX_Z);

		Mesh& tile_mesh = pool::get_mesh(tile_mesh_name, true);
		expand_floor(&tile_mesh, 1.0f, &floor_mesh, tiles);
		//floor_mesh = &tile_mesh;

		floor = new RenderObject(*floor_mesh);
	}

	WorldRenderer::~WorldRenderer() {
		delete floor_mesh;
		delete floor;

		/* // not necessary: the map is destroyed with this object, it's an automatic variable
		auto iter = objects.begin();
		while(iter != objects.end()) {
			objects.erase(iter);
			iter = objects.begin();
		}
		*/
	}


	void WorldRenderer::setView(glm::vec3 pos, GLfloat yaw, GLfloat pitch) {
		pos.x = gla::within_bounds(pos.x, -floor_tiles_half, floor_tiles_half);
		pos.z = gla::within_bounds(pos.z, -floor_tiles_half, floor_tiles_half);
		view_pos = pos;
		view_yaw = yaw;
		view_pitch = pitch;
	}

	void WorldRenderer::putObject(WorldObject& obj) {
		objects[obj.uid] = &obj;
		//std::cout << "now rendering " << obj.uid << std::endl;
	}

	void WorldRenderer::removeObject(WorldObject& obj) {
		removeObject(obj.uid);
		//std::cout << "not rendering " << obj.uid << std::endl;
	}

	void WorldRenderer::removeObject(std::size_t uid) {
		auto iter = objects.find(uid);
		if(iter != objects.end())
			objects.erase(iter);
	}

	void WorldRenderer::renderFrame() {
		glClearColor(clear_color[0], clear_color[1], clear_color[2], 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform1f(pool::uniform_curvature, curvature);
		glUniformMatrix4fv(pool::uniform_proj, 1, GL_FALSE, &mat_proj[0][0]);

		glm::mat4 mat_view;
		/* can be optimized: everything is drawn 9 times, but a
		 * maximum of 4 iterations can theoretically suffice */
		for(int x=-1; x<2; x+=1) {
			for(int z=-1; z<2; z+=1) {
				/* can be optimized: matrix only has to be translated, but gets
				 * rotated too */
				mat_view = get_view(
						glm::vec3(
							view_pos[0] + (x * floor_tiles),
							view_pos[1],
							view_pos[2] + (z * floor_tiles)
						),
						view_yaw, view_pitch );

				glUniformMatrix4fv(pool::uniform_view, 1, GL_FALSE, &mat_view[0][0]);

				floor->draw();

				auto iter = objects.begin();
				while(iter != objects.end()) {
					iter->second->draw();
					++iter;
				}
			}
		}

		SDL_GL_SwapWindow(sneka::pool::runtime()->window);
	}

}
