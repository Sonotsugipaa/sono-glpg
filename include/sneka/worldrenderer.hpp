#ifndef SNEKA_WORLDRENDERER_HPP
#define SNEKA_WORLDRENDERER_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "sneka/renderer.hpp"
#include "sneka/renderobject.hpp"
#include "sneka/floorobject.hpp"

#include "utils.tpp"

#include <map>



namespace sneka {

	class WorldRenderer : public Renderer {
	protected:
		gla::Timer time;
		FloorObject* floor;

		std::map<uid_t, RenderObject*> objects;
		GLfloat floor_tiles, floor_tiles_half, floor_tile_size;
		GLfloat repeat_stride;
		GLfloat curvature, drugs;

		glm::vec3 light_direction;

		glm::vec3 view_pos;
		GLfloat view_yaw, view_pitch;

		GLfloat proj_fov_y, proj_z_near, proj_z_far;

	public:
		glm::vec3 clear_color;
		GLfloat fog_intensity;

		WorldRenderer(
				FloorObject& floor, GLuint repeat_stride,
				GLfloat curvature, GLfloat drugs,
				GLuint screen_width, GLuint screen_height );
		virtual ~WorldRenderer();

		/* Returns the pointer to a random object, if there is any;
		 * 'nullptr' otherwise. */
		RenderObject* popObject();
		RenderObject* getObject(uid_t);
		void putObject(RenderObject& object);
		void removeObject(RenderObject& object);
		void removeObject(uid_t);
		std::size_t size();

		FloorObject & getFloorObject();

		void setLightDirection(glm::vec3);
		void setView(glm::vec3 position, GLfloat yaw, GLfloat pitch);

		void setWorldPerspective(
				GLfloat fov_y,
				GLfloat zNear, GLfloat zFar
		);

		virtual void renderFrame();
	};

}

#endif
