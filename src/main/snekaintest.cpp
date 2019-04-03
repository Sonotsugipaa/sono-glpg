#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <exception>

#include <cstdlib>
#include <cstdio>

#include "sneka/shaders.hpp"
#include "sneka/renderobject.hpp"
#include "sneka/direction.hpp"
#include "sneka/gridobject.hpp"
#include "sneka/levelrenderer.hpp"
#include "sneka/asset.hpp"

#include "runtime.hpp"
#include "shader.hpp"
#include "globject.hpp"
#include "transition.hpp"
#include "utils.tpp"
#include "read_utils.hpp"
#include "trace.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH         (1000)
#define WINDOW_SIZE_FACTOR   (0.7)
#define FPS                  (60.0)
#define FRAMERATE            (1000.0/FPS)
#define FRAMES               (500)
#define CAM_DISTANCE         (1.0)
#define CAM_HEIGHT           (2.0)
#define CAM_PITCH            (1.1)
#define CURVATURE            (-0.03)
#define TILES                (25)
#define OBJECTS              (250)
#define DRUGS                (0.0)
#define WORLD_MIN_Z          (0.2)
#define WORLD_MAX_Z          (100.0)

#define OBJECT_SHADE         (0.7)
#define OBJECT_REFLECT       (0.99)
#define OBJECT_REFLECT_FO    (3.0)
#define OBJECT_REFLECT_O     (0.2)
#define OBJECT_REFLECT_N     (-0.0)
#define FLOOR_SHADE          (0.6)
#define FLOOR_REFLECT        (0.2)
#define FLOOR_REFLECT_FO     (2.0)
#define FLOOR_REFLECT_O      (1.7)
#define FLOOR_REFLECT_N      (-0.0)

#define COLOR_SKY            0.1,0.1,0.1
#define COLOR_LIGHT          1.0,1.0,1.0

using std::cout;
using std::endl;



namespace {

	using namespace sneka;

	glm::vec2 pos, pos_target;
	glm::ivec2 ipos_target;
	std::map<int unsigned, GridObject*> grid_objects;
	RenderObject* head;
	const GLfloat speed = 2.5f;
	GLfloat speed_boost = 0.0f;
	GLfloat accel = 0.0f;
	GLfloat rot = 0.0f;
	Direction direction;

	LevelRenderer* renderer;
	MeshLoader mesh_loader;

	gla::Timer frame_timer;


	struct Movement {
	private:
		void update_diff() {
			xdiff = pos_target[0] - old_pos[0];
			zdiff = pos_target[1] - old_pos[1];

			rdiff = direction.radians() - old_rot;
			if(rdiff > 3.1415f)
				rdiff -= 6.2830f;
			else
			if(rdiff < -3.1415f)
				rdiff += 6.2830f;
		}

	public:
		glm::vec2 old_pos;
		float xdiff;
		float zdiff;

		Direction old_direction;
		float old_rot;
		float rdiff;

		Movement():
				old_pos(pos),
				old_direction(direction),
				old_rot(direction.radians())
		{
			update_diff();
		}

		void update() {
			old_pos = pos;

			if(rot > 6.2830f)
				rot -= 6.2830f;
			else
			if(rot < 0.0f)
				rot += 6.2830f;
			old_rot = rot;

			update_diff();
		}
	} movement;


	void transition_pos(float adv) {
		pos[0] = movement.old_pos[0] + (movement.xdiff * adv);
		pos[1] = movement.old_pos[1] + (movement.zdiff * adv);
		rot = movement.old_rot + (movement.rdiff * adv);

		/* room for improvement; the head stutters because it's not
		 * synced with the view matrix */
		head->setPosition(pos[0], 0.0f, pos[1]);
	}


	/*
	std::ostream& operator << (std::ostream& stream, const glm::vec2 & vec) {
		stream << '[' << vec[0] << ", " << vec[1] << ']';
		return stream;
	}
	std::ostream& operator << (std::ostream& stream, const glm::vec3 & vec) {
		stream << '[' << vec[0] << ", " << vec[1] << ", " << vec[2] << ']';
		return stream;
	}
	std::ostream& operator << (std::ostream& stream, const glm::vec4 & vec) {
		stream << '[' << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ']';
		return stream;
	}
	*/


	int unsigned hash_ivec2(glm::ivec2& v) {
		return
				((v[0] << (4 * sizeof(unsigned int))) >> (4 * sizeof(unsigned int))) |
				( v[1] << (4 * sizeof(unsigned int)));
	}

	// Generates random objects through the map.
	void genObjects(
			LevelRenderer* renderer,
			Mesh& mesh, std::size_t count,
			int unsigned tiles,
			GLfloat shade,
			GLfloat reflect, GLfloat reflect_falloff,
			GLfloat reflect_opaque, GLfloat reflect_negative
	) {
		using namespace std::chrono;

		system_clock::duration time = system_clock::now().time_since_epoch();
		unsigned int rand = time.count();
		unsigned int genj = 0;

		for(std::size_t i=0; i < count; i+=1) {
			GridObject* newobj = new GridObject(mesh);

			glm::ivec2 genpos;
			glm::vec4 gencol;  gencol[3] = 1.0f;
			int unsigned genhash;
			int unsigned geni = 0;
			do {
				genpos = glm::ivec2(
						gla::xorshift(rand + count + i + genj) % (unsigned int) tiles,
						gla::xorshift(rand + i - count + genj) % (unsigned int) tiles );
				genhash = hash_ivec2(genpos);
				geni += 1;
				genj += geni;
				if(geni > 1000)
					throw std::runtime_error(
							"Could not generate a random position for an object"
							" (too many attempts)" );
			} while(grid_objects.find(genhash) != grid_objects.end());

			//gencol[0] = (float) gla::xorshift<unsigned char>(genj  );
			//gencol[1] = (float) gla::xorshift<unsigned char>(genj+1);
			//gencol[2] = (float) gla::xorshift<unsigned char>(genj+2);
			//gencol[3] = (float) gla::xorshift<unsigned char>(genj+3);
			gencol[0] = 0.7f;
			gencol[1] = 0.5f;
			gencol[2] = 0.0f;

			newobj->setGridPosition(genpos);
			newobj->setRotation((Direction::FORWARD + Direction::LEFT).radians() * genj);
			newobj->setColor(gencol);
			newobj->shade = shade;
			newobj->reflect = reflect;
			newobj->reflect_falloff = reflect_falloff;
			newobj->reflect_opaque = reflect_opaque;
			newobj->reflect_negative = reflect_negative;
			grid_objects[genhash] = newobj;
			renderer->putObject(*newobj);
		}
	}

	void destroyObjects(LevelRenderer* renderer) {
		RenderObject* obj;
		auto iter = grid_objects.begin();
		auto end = grid_objects.end();

		while(iter != end) {
			obj = iter->second;
			renderer->removeObject(obj->getUid());
			grid_objects.erase(iter);
			delete obj;

			iter = grid_objects.begin();
		}
	}

}


int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	int W = SCREEN_WIDTH, H = SCREEN_WIDTH * 9 / 16;
	/*
	{
		SDL_DisplayMode dm;
		if(SDL_GetDesktopDisplayMode(0, &dm) == 0) {
			W = dm.w * WINDOW_SIZE_FACTOR;
			H = dm.h * WINDOW_SIZE_FACTOR;
		}
	}
	*/

	if(argn > 1) {
		int read;
		read_dec_int(args[1], &read);
		if(read > 0) {
			W = read;
			H = W * 9 / 16;
		}
	}
	if(argn > 2) {
		int read;
		read_dec_int(args[2], &read);
		if(read > 0) {
			H = read;
		}
	}

	trace_sigaction_init();  TRACE;

	SnekaRuntime runtime = SnekaRuntime(
			"sneka level render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true );  TRACE;
	/*
	pool::runtime_init(
			"sneka level render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);  TRACE;
	*/

	FloorObject floor = FloorObject(mesh_loader.get("assets/tile_caved.mesh"), 40);

	renderer = new LevelRenderer(
			runtime,
			floor, TILES,
			CURVATURE, DRUGS );  TRACE;
	renderer->setClearColor(glm::vec3(COLOR_SKY));
	renderer->setLightColor(glm::vec3(COLOR_LIGHT));
	renderer->getFloorObject().setColor(glm::vec4(COLOR_SKY, 1.0f));
	renderer->getFloorObject().shade = (float) FLOOR_SHADE;
	renderer->getFloorObject().reflect = (float) FLOOR_REFLECT;
	renderer->getFloorObject().reflect_falloff = (float) FLOOR_REFLECT_FO;
	renderer->getFloorObject().reflect_opaque = (float) FLOOR_REFLECT_O;
	renderer->getFloorObject().reflect_negative = (float) FLOOR_REFLECT_N;
	renderer->addLight(glm::vec3( 1.0f, 0.5f,  3.0f));
	renderer->addLight(glm::vec3( 3.0f, 0.5f, -3.0f));
	renderer->addLight(glm::vec3(-1.0f, 1.0f,  0.0f));
	renderer->setPerspective(
					90.0f,
					(GLfloat) WORLD_MIN_Z,
					(GLfloat) WORLD_MAX_Z);
	TRACE;

	// is deallocated automatically by destroyObjects(...)
	head = new RenderObject(mesh_loader.get("assets/arrow.mesh"));
	renderer->putObject(*head);

	genObjects(
			renderer,
			mesh_loader.get("assets/pyrg.mesh"),  OBJECTS / 2, TILES,
			(float) OBJECT_SHADE,
			(float) OBJECT_REFLECT,
			(float) OBJECT_REFLECT_FO,
			(float) OBJECT_REFLECT_O,
			(float) OBJECT_REFLECT_N );
	genObjects(
			renderer,
			mesh_loader.get("assets/blocg.mesh"), OBJECTS / 4, TILES,
			(float) OBJECT_SHADE,
			(float) OBJECT_REFLECT,
			(float) OBJECT_REFLECT_FO,
			(float) OBJECT_REFLECT_O,
			(float) OBJECT_REFLECT_N );
	genObjects(
			renderer,
			mesh_loader.get("assets/bloc.mesh"), OBJECTS / 4, TILES,
			(float) OBJECT_SHADE,
			(float) OBJECT_REFLECT,
			(float) OBJECT_REFLECT_FO,
			(float) OBJECT_REFLECT_O,
			(float) OBJECT_REFLECT_N );

	runtime.setResizeCallback( [](SnekaRuntime& rt, unsigned int x, unsigned int y) {
		rt.setViewport(x, y);
	} );  TRACE;

	runtime.setKeyCallback( [](
			SnekaRuntime& rt [[maybe_unused]],
			unsigned int keycode,
			unsigned int mod [[maybe_unused]],
			bool released
	) {
		if(! released) {
			switch(keycode) {
				//case SDLK_w:  direction += Direction::FORWARD;   break;
				//case SDLK_s:  direction += Direction::BACKWARD;  break;
				case SDLK_w:  accel =  1.3f;  break;
				case SDLK_s:  accel = -0.5f;  break;
				case SDLK_d:  direction += Direction::RIGHT;  break;
				case SDLK_a:  direction += Direction::LEFT;   break;
			}
		} else {
			switch(keycode) {
			case SDLK_w:
			case SDLK_s:  accel = 0.0f;  break;
			}
		}
	} );  TRACE;

	Transition movement_tr = Transition(
			transition_pos,
			1.0f );  TRACE;

	while(runtime.pollEvents()) {
		if(direction == Direction::FORWARD)  ipos_target[1] -= 1; else
		if(direction == Direction::BACKWARD) ipos_target[1] += 1; else
		if(direction == Direction::RIGHT)    ipos_target[0] += 1; else
		if(direction == Direction::LEFT)     ipos_target[0] -= 1;

		pos_target = ipos_target;

		// normalize, to keep within bounds of the floor
		if(ipos_target[0] > TILES) {
			pos[0]         -= TILES;
			ipos_target[0] -= TILES;
			pos_target[0]  -= TILES;
		}
		if(ipos_target[0] < 0) {
			pos[0]         += TILES;
			ipos_target[0] += TILES;
			pos_target[0]  += TILES;
		}
		if(ipos_target[1] > TILES) {
			pos[1]         -= TILES;
			ipos_target[1] -= TILES;
			pos_target[1]  -= TILES;
		}
		if(ipos_target[1] < 0) {
			pos[1]         += TILES;
			ipos_target[1] += TILES;
			pos_target[1]  += TILES;
		}

		if(speed_boost > 0.0f) {
			speed_boost /= 1.1f;
		}
		else
		if(speed_boost < 0.0f) {
			speed_boost /= 1.7f;
		}
		speed_boost += accel;
		TRACE;
		/*
		std::cout << "dir     " << direction.degrees() << std::endl;
		std::cout << "dir old " << movement.old_direction.degrees() << std::endl;
		*/

		movement.update();
		head->setRotation(direction.radians());

		frame_timer.reset();
		bool cycle_frame_timer = true;
		float speed_total = speed + speed_boost;
		TRACE;
		while(cycle_frame_timer) {
			TRACE;
			float delta = speed_total * frame_timer.s();
			frame_timer.reset();
			cycle_frame_timer = movement_tr.advance(delta);

			// delays a bit more than the actual framerate
			std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));
			renderer->setView(
					// wtf math
					glm::vec3(
						-((CAM_DISTANCE * glm::sin(rot)) + pos[0]),
						-CAM_HEIGHT,
						-((CAM_DISTANCE * glm::cos(rot)) + pos[1]) ),
					-rot,
					CAM_PITCH );
			renderer->renderFrame();
		}
	}

	TRACE;
	destroyObjects(renderer);
	delete renderer;

	TRACE;
	//pool::runtime_destroy();
	return EXIT_SUCCESS;
}
