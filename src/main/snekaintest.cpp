#include <iostream>
#include <thread>
#include <chrono>
#include <map>

#include <cstdlib>
#include <cstdio>

#include "sneka/pool.hpp"
#include "sneka/renderobject.hpp"
#include "sneka/world.hpp"

#include "runtime.hpp"
#include "shader.hpp"
#include "globject.hpp"
#include "transition.hpp"
#include "utils.tpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH   (1600)
#define FPS            (60.0)
#define FRAMERATE      (1000.0/FPS)
#define STEPS          (10)
#define FRAMES         (500)
#define CAM_DISTANCE   (1.0)
#define CAM_HEIGHT     (2.0)
#define CAM_PITCH      (1.1)
#define CURVATURE      (-0.03)
#define TILES          (40)
#define OBJECTS        (500)



namespace {

	using namespace sneka;

	glm::vec2 pos, pos_target;
	glm::ivec2 ipos_target;
	RenderObject* head;
	const GLfloat speed = 2.5f;
	GLfloat speed_boost = 0.0f;
	GLfloat rot = 0.0f;
	Direction direction;

	WorldRenderer* renderer;

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

	unsigned int xorshift(unsigned int x) {
		x ^= (x << 13) ^ (x >> 13);
		x ^= (x << 2) ^ (x >> 2);
		x ^= (x << 20) ^ (x >> 20);
		return x;
	}

	// Generates random objects through the map.
	void genObjects(WorldRenderer* renderer, std::string mesh, std::size_t count) {
		using namespace std::chrono;

		system_clock::duration time = system_clock::now().time_since_epoch();
		unsigned int rand = time.count();

		for(std::size_t i=0; i < count; i+=1) {
			GridObject* newobj = new GridObject(mesh);
			newobj->setGridPosition(
					xorshift(rand + count + i) % (unsigned int) TILES,
					xorshift(rand + i - count) % (unsigned int) TILES );
			newobj->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
			renderer->putObject(*newobj);
		}
	}

	void destroyObjects(WorldRenderer* renderer) {
		RenderObject* obj;
		while((obj = renderer->popObject()) != nullptr) {
			renderer->removeObject(obj->uid);
		}
	}

}


int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	const int W = SCREEN_WIDTH, H = SCREEN_WIDTH * 9 / 16;

	pool::runtime_init(
			"sneka world render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);

	renderer = new WorldRenderer(
			"assets/tile_caved.mesh", TILES,
			CURVATURE, 0.5f, W, H );
	renderer->clear_color = glm::vec3(0.4f, 0.4f, 0.7f);
	renderer->getFloorObject().setColor(glm::vec4(0.4f, 0.7f, 0.4f, 1.0f));
	renderer->setLightDirection(glm::vec3(2.0f, 1.0f, 0.0f));

	// is deallocated automatically by destroyObjects(...)
	head = new RenderObject("assets/arrow.mesh");
	renderer->putObject(*head);

	genObjects(renderer, "assets/pyr.mesh", OBJECTS);

	pool::set_key_callback( [](unsigned int keycode, unsigned int mod, bool released) {
		if(! released) {
			switch(keycode) {
				//case SDLK_w:  direction += Direction::FORWARD;   break;
				//case SDLK_s:  direction += Direction::BACKWARD;  break;
				case SDLK_w:  speed_boost = speed *  6.0f;    break;
				case SDLK_s:  speed_boost = speed * -0.7f;    break;
				case SDLK_d:  direction += Direction::RIGHT;  break;
				case SDLK_a:  direction += Direction::LEFT;   break;
			}
		}
	} );

	Transition movement_tr = Transition(
			transition_pos,
			1.0f );

	while(pool::poll_events()) {
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

		/*
		std::cout << "dir     " << direction.degrees() << std::endl;
		std::cout << "dir old " << movement.old_direction.degrees() << std::endl;
		*/

		movement.update();
		head->setRotation(direction.radians());

		frame_timer.reset();
		bool cycle_frame_timer = true;
		float speed_total = speed + speed_boost;
		while(cycle_frame_timer) {
			float delta = speed_total * frame_timer.millis() / 1000.0f;
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

	destroyObjects(renderer);
	delete renderer;

	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
