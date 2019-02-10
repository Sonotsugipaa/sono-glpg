#ifndef SNEKA_WORLD_HPP
#define SNEKA_WORLD_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "sneka/renderer.hpp"
#include "sneka/renderobject.hpp"

#include <map>

#define WORLD_MIN_Z (0.2)
#define WORLD_MAX_Z (100.0)



namespace sneka {

	using uid_t = std::size_t;

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
	};


	class WorldObject : public virtual RenderObject {
	protected:
		glm::ivec2 grid_pos;
		GLfloat height = 0.0f;
	public:
		const uid_t uid;

		WorldObject(Mesh&);
		WorldObject(std::string mesh_name);
		virtual ~WorldObject() = default;

		void setHeight(GLfloat);

		void setGridPosition(glm::ivec2);
		void setGridPosition(GLint x, GLint z);
	};

	class WorldRenderer : public virtual Renderer {
	protected:
		Mesh* floor_mesh;
		RenderObject* floor;

		std::map<uid_t, WorldObject*> objects;
		GLfloat floor_tiles, floor_tiles_half, curvature, drugs;

		glm::mat4 mat_proj;

		glm::vec3 view_pos;
		GLfloat view_yaw, view_pitch;

	public:
		glm::vec3 clear_color = glm::vec3(0.05f, 0.05f, 0.05f);

		WorldRenderer(
				std::string tile_mesh_name,
				GLuint floor_tiles, GLfloat curvature, GLfloat drugs,
				GLuint screen_width, GLuint screen_height );
		virtual ~WorldRenderer();

		/* Returns the pointer to a random object, if there is any;
		 * 'nullptr' otherwise. */
		WorldObject* popObject();
		WorldObject* getObject(uid_t);
		void putObject(WorldObject& object);
		void removeObject(WorldObject& object);
		void removeObject(uid_t);
		std::size_t size();

		void setView(glm::vec3 position, GLfloat yaw, GLfloat pitch);

		virtual void renderFrame();
	};

}

#endif
