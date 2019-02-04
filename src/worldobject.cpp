#include "sneka/world.hpp"



namespace {

	std::size_t gen = 1;

}


namespace sneka {

	WorldObject::WorldObject(Mesh& mesh):
			RenderObject::RenderObject(mesh), uid(gen++)
	{ }

	WorldObject::WorldObject(std::string mesh_name):
			RenderObject::RenderObject(mesh_name), uid(gen++)
	{ }


	void WorldObject::setHeight(GLfloat h) {
		height = h;

		RenderObject::setPosition(grid_pos[0], height, grid_pos[1]);
	}

	void WorldObject::setGridPosition(glm::ivec2 pos) {
		grid_pos = pos;

		RenderObject::setPosition(pos[0], height, pos[1]);
	}

	void WorldObject::setGridPosition(GLint x, GLint z) {
		grid_pos[0] = x;
		grid_pos[1] = z;

		RenderObject::setPosition(x, height, z);
	}

}
