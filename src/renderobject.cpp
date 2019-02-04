#include "sneka/renderobject.hpp"

#include "sneka/pool.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "debug.hpp"



namespace sneka {

	RenderObject::RenderObject(Mesh& mesh):
			mesh(mesh)
	{ }

	RenderObject::RenderObject(std::string mesh_name):
			RenderObject::RenderObject(pool::get_mesh(mesh_name))
	{ }


	void RenderObject::mat_compute() {
		mat_model = glm::rotate(
				glm::translate(glm::mat4(1.0f), position),
				rotation,
				glm::vec3(0.0f, 1.0f, 0.0f) );
	}


	void RenderObject::setPosition(glm::vec3 pos) {
		position = pos;
		mat_changed = true;
	}

	void RenderObject::setPosition(GLfloat x, GLfloat y, GLfloat z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		mat_changed = true;
	}

	void RenderObject::setRotationRad(GLfloat r) {
		rotation = r;
		mat_changed = true;
	}

	void RenderObject::setRotationDeg(GLfloat r) {
		setRotationRad(r * (3.141592f / 180.0f));
	}

	void RenderObject::setColor(glm::vec4 col) {
		color_add = true;
		color = col;
	}

	void RenderObject::draw() {
		pool::set_add_col_enabled(color_add);

		if(color_add)
			glUniformMatrix4fv(pool::uniform_add_col, 1, GL_FALSE, &color[0]);

		if(mat_changed) {
			mat_compute();
		}

		glUniformMatrix4fv(
				sneka::pool::uniform_model,
				1,
				GL_FALSE,
				&mat_model[0][0] );

		mesh.draw();
	}

}
