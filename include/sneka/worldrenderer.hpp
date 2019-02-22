#ifndef SNEKA_WORLDRENDERER_HPP
#define SNEKA_WORLDRENDERER_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "sneka/renderer.hpp"
#include "sneka/renderobject.hpp"

#include "utils.tpp"

#include <map>



namespace sneka {

	class WorldRenderer : public virtual Renderer {
	protected:
		gla::Timer time;
		Mesh* floor_mesh;
		RenderObject* floor;

		std::map<uid_t, RenderObject*> objects;
		GLfloat floor_tiles, floor_tiles_half, floor_tile_size, curvature, drugs;

		glm::vec3 light_direction;

		glm::vec3 view_pos;
		GLfloat view_yaw, view_pitch;

	public:
		glm::vec3 clear_color;
		GLfloat fog_intensity;

		WorldRenderer(
				std::string tile_mesh_name,
				GLuint floor_tiles, GLfloat curvature, GLfloat drugs,
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

		RenderObject& getFloorObject();

		void setLightDirection(glm::vec3);
		void setView(glm::vec3 position, GLfloat yaw, GLfloat pitch);

		virtual void renderFrame();
	};

}

#endif
