#ifndef SNEKA_GRIDOBJECT_HPP
#define SNEKA_GRIDOBJECT_HPP

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "sneka/renderobject.hpp"



namespace sneka {

	class GridObject : public virtual RenderObject {
	private:
		glm::vec2 cache_combined;
		bool changed = true;
		glm::ivec2 grid_pos;
		glm::vec2 offset_pos;
		float height = 0.0f;

	protected:
		void updatePosition();

	public:

		GridObject(Mesh&);
		GridObject(std::string mesh_name);
		virtual ~GridObject() = default;

		void setHeight(float);

		void setGridPosition(glm::ivec2);
		void setGridPosition(GLint x, GLint z);

		void setOffsetPosition(glm::vec2);
		void setOffsetPosition(float x, float z);

		virtual void draw();
	};

}

#endif
