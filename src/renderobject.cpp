#include "sneka/renderobject.hpp"

#include "sneka/pool.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "debug.hpp"



namespace {

	std::size_t gen = 1;

}


namespace sneka {

	RenderObject::RenderObject(Mesh& mesh):
			mesh(mesh),
			mat_changed(true),
			color_mul(false),
			position(0.0f),
			translation(0.0f),
			rotation(0.0f),
			uid(gen++),
			shade(0.3f)
	{ }

	RenderObject::RenderObject(std::string mesh_name):
			RenderObject::RenderObject(pool::get_mesh(mesh_name))
	{ }


	void RenderObject::mat_compute() {
		mat_model = glm::rotate(
				glm::translate(glm::mat4(1.0f), translation),
				rotation,
				glm::vec3(0.0f, 1.0f, 0.0f) );
	}


	glm::vec3 RenderObject::getPosition() {
		return position;
	}

	void RenderObject::setPosition(glm::vec3 pos) {
		position = pos;
		//mat_changed = true;  // translation done by the renderer
	}

	void RenderObject::setPosition(GLfloat x, GLfloat y, GLfloat z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		//mat_changed = true;  // translation done by the renderer
	}

	GLfloat RenderObject::getRotation() {
		return rotation;
	}


	void RenderObject::setRotation(GLfloat r) {
		rotation = r;
		mat_changed = true;
	}

	void RenderObject::setColor(glm::vec4 col) {
		color_mul = true;
		color = col;
	}

	void RenderObject::draw() {
		pool::set_mul_col_enabled(color_mul);

		if(color_mul)
			glUniform4fv(pool::uniform_mul_col, 1, &color[0]);

		if(mat_changed) {
			mat_compute();
		}

		glUniform3fv(pool::uniform_model_pos, 1, &position[0]);
		glUniform1f(pool::uniform_shade, shade);
		glUniformMatrix4fv(
				sneka::pool::uniform_model,
				1,
				GL_FALSE,
				&mat_model[0][0] );

		mesh.draw();
	}

}
