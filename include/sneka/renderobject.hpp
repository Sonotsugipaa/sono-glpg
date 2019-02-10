#ifndef SNEKA_OBJECT_HPP
#define SNEKA_OBJECT_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "globject.hpp"

#include "sneka/mesh.hpp"



namespace sneka {

	using uid_t = std::size_t;

	class RenderObject {
	protected:
		Mesh& mesh;
		glm::mat4 mat_model;
		bool mat_changed = true;
		bool color_add = false;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec4 color;
		GLfloat rotation = 0.0f; // +Y axis

		void mat_compute();

	public:
		const uid_t uid;

		RenderObject(Mesh&);
		RenderObject(std::string mesh_name);
		virtual ~RenderObject() = default;

		void setPosition(glm::vec3);
		void setPosition(GLfloat x, GLfloat y, GLfloat z);

		void setRotationRad(GLfloat);
		void setRotationDeg(GLfloat);

		void setColor(glm::vec4);

		virtual void draw();
	};

}

#endif
