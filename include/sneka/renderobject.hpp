#ifndef SNEKA_OBJECT_HPP
#define SNEKA_OBJECT_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "globject.hpp"

#include "sneka/mesh.hpp"
#include "sneka/snekaruntime.hpp"



namespace sneka {

	using uid_t = std::size_t;

	class RenderObject {
	protected:
		const Mesh * mesh;
		glm::mat4 mat_model;
		bool mat_changed;
		bool color_mul;

		/* 'position' is the position in the world,
		 * 'translation' the position relative to the center
		 * of the model */
		glm::vec3 position;
		glm::vec3 translation;

		glm::vec4 color;
		GLfloat rotation; // +Y axis

		void mat_compute();

		// used if a subclass wants to build and allocate its own mesh
		RenderObject();

	public:
		const uid_t uid;

		GLfloat
			/* darkens the object */
			shade,
			/* reflection intensity */
			reflect,
			/* exponent for the reflection; higher values
			 * result in sharper reflections */
			reflect_falloff,
			/* reflect light regardless of the viewer's
			 * position */
			reflect_opaque,
			/* lower limit for the reflection
			 * (decrease for a metallic feel, increase for
			 * a "forced reflection" (which doesn't really
			 * make sense)) */
			reflect_negative;

		RenderObject(const Mesh &);
		virtual ~RenderObject() = default;

		glm::vec3 getPosition() const;
		void setPosition(glm::vec3);
		void setPosition(GLfloat x, GLfloat y, GLfloat z);

		GLfloat getRotation() const;
		void setRotation(GLfloat);

		glm::vec4 getColor() const;
		bool isColorEnabled() const;
		void setColorEnabled(bool);
		void setColor(glm::vec4);

		const Mesh * getMesh() const;
		virtual void draw();
	};

}

#endif
