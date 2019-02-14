#ifndef SNEKA_WORLD_HPP
#define SNEKA_WORLD_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "sneka/renderer.hpp"
#include "sneka/renderobject.hpp"

#include "utils.tpp"

#include <map>



namespace sneka {

	class Direction {
	private:
		int value;

		Direction(int);
	public:
		static const Direction FORWARD, LEFT, BACKWARD, RIGHT;

		Direction() = default;
		Direction(const Direction &) = default;
		Direction(Direction &&) = default;

		// "moves" the direction
		Direction combined(const Direction &) const;
		Direction operator + (const Direction &) const;

		Direction relative(const Direction &) const;
		Direction operator - (const Direction &) const;
		bool operator == (const Direction &) const;

		Direction& operator = (const Direction &) = default;
		Direction& operator = (Direction &&) = default;

		Direction& operator += (const Direction &);
		Direction& operator += (Direction&&);
		Direction& operator -= (const Direction &);
		Direction& operator -= (Direction&&);


		GLfloat radians() const;
		GLfloat degrees() const;
		GLfloat sin() const;
		GLfloat cos() const;
	};


	class GridObject : public virtual RenderObject {
	protected:
		glm::ivec2 grid_pos;
		glm::vec2 offset_pos;
		glm::vec2 cache_combined;
		GLfloat height = 0.0f;
		bool changed = true;

		void updatePosition();

	public:

		GridObject(Mesh&);
		GridObject(std::string mesh_name);
		virtual ~GridObject() = default;

		void setHeight(GLfloat);

		void setGridPosition(glm::ivec2);
		void setGridPosition(GLint x, GLint z);

		void setOffsetPosition(glm::vec2);
		void setOffsetPosition(GLfloat x, GLfloat z);

		virtual void draw();
	};

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
