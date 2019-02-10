#include "sneka/world.hpp"



namespace sneka {

	GridObject::GridObject(Mesh& mesh):
			RenderObject::RenderObject(mesh)
	{ }

	GridObject::GridObject(std::string mesh_name):
			RenderObject::RenderObject(mesh_name)
	{ }


	void GridObject::setHeight(GLfloat h) {
		height = h;

		changed = true;
	}

	void GridObject::setGridPosition(glm::ivec2 pos) {
		grid_pos = pos;

		changed = true;
	}

	void GridObject::setGridPosition(GLint x, GLint z) {
		grid_pos[0] = x;
		grid_pos[1] = z;

		changed = true;
	}

	void GridObject::setOffsetPosition(glm::vec2 vec) {
		offset_pos = vec;

		changed = true;
	}

	void GridObject::setOffsetPosition(GLfloat x, GLfloat z) {
		offset_pos = glm::vec2(x, z);

		changed = true;
	}


	void GridObject::updatePosition() {
		if(! changed) return;

		glm::vec2 combined = glm::vec2(
				offset_pos[0] + static_cast<GLfloat>(grid_pos[0]),
				offset_pos[1] + static_cast<GLfloat>(grid_pos[1]) );
		RenderObject::setPosition(combined[0], height, combined[1]);

		changed = false;
	}

	void GridObject::draw() {
		updatePosition();
		RenderObject::draw();
	}

}
