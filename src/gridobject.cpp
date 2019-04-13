#include "sneka/gridobject.hpp"



namespace sneka {

	GridObject::GridObject(Mesh& mesh):
			RenderObject::RenderObject(mesh),
			grid_pos(0),
			offset_pos(0.0f)
	{ }


	void GridObject::setHeight(float h) {
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

	void GridObject::setOffsetPosition(float x, float z) {
		offset_pos = glm::vec2(x, z);

		changed = true;
	}


	void GridObject::updatePosition() {
		if(! changed) return;

		glm::vec2 combined = glm::vec2(
				offset_pos[0] + static_cast<float>(grid_pos[0]),
				offset_pos[1] + static_cast<float>(grid_pos[1]) );
		RenderObject::setPosition(combined[0], height, combined[1]);

		changed = false;
	}

	void GridObject::draw() {
		updatePosition();
		RenderObject::draw();
	}

}