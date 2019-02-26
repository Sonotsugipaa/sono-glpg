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
	public:
		// this value depends on the vertex shader
		static constexpr GLint max_lights = 8;

	private:
		GLfloat lights_cache[max_lights * 3];
		bool lights_cache_changed;
		void lights_cache_compute();

	protected:
		gla::Timer time;
		FloorObject* floor;

		std::map<uid_t, RenderObject*> objects;
		GLfloat floor_tiles, floor_tiles_half, floor_tile_size;
		GLfloat repeat_stride;
		GLfloat curvature, drugs;

		std::vector<glm::vec3> lights;

		glm::vec3 light_color;
		glm::vec3 clear_color;

		glm::vec3 view_pos;
		GLfloat view_yaw, view_pitch;

		GLfloat proj_fov_y, proj_z_near, proj_z_far;

	public:
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
		const RenderObject * getObject(uid_t) const;
		void putObject(RenderObject& object);
		void removeObject(RenderObject& object);
		void removeObject(uid_t);
		std::size_t getObjectsCount() const;

		FloorObject & getFloorObject();

		/* The maximum number of lights is defined in WorldRenderer::max_lights;
		 * trying to add more lights than that will have no effect.
		 * Returns 'true' if the light was successfully added. */
		bool addLight(glm::vec3 direction);
		/* Sets the 'index'th light to the specified value;
		 * if the index is out of bounds with an offset of 1 a new
		 * light is added; otherwise 'false' is returned and
		 * the light is not set. */
		bool setLight(std::size_t index, glm::vec3 direction);
		void clearLights();
		std::vector<glm::vec3> getLights() const;
		std::size_t getLightsCount() const;

		glm::vec3 getLightColor() const;
		glm::vec3 getClearColor() const;
		void setLightColor(glm::vec3);
		void setClearColor(glm::vec3);

		void setView(glm::vec3 position, GLfloat yaw, GLfloat pitch);

		void setWorldPerspective(
				GLfloat fov_y,
				GLfloat zNear, GLfloat zFar
		);

		virtual void renderFrame();
	};

}

#endif
