#include <iostream>
#include <thread>
#include <chrono>

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

#define SCREEN_WIDTH   (1800)
#define TILES          (20.0)
#define FPS            (60.0)
#define FRAMERATE      (1000.0/FPS)
#define STEPS          (10)
#define FRAMES         (500)



namespace {

	using namespace sneka;

	glm::vec2 pos, pos_target;
	const GLfloat speed = 1.5f;
	GLfloat speed_boost = 0.0f;
	GLfloat objrot = 0.0f;
	GLfloat rot = 0.0f;
	Direction direction;

	WorldRenderer* renderer;
	WorldObject* wobj1;
	WorldObject* wobj2;

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
			"assets/tile.mesh", TILES,
			-0.03f, 0, W, H );
	renderer->clear_color = glm::vec3(0.05f, 0.05f, 0.15f);

	wobj1 = new WorldObject("assets/trisquare.mesh");
	wobj1->setGridPosition(1, 0);
	wobj1->setHeight(0.2f);
	renderer->putObject(*wobj1);
	wobj2 = new WorldObject("assets/trisquare.mesh");
	wobj2->setGridPosition(-1, 0);
	wobj2->setHeight(0.2f);
	renderer->putObject(*wobj2);

	pool::set_key_callback( [](unsigned int keycode, unsigned int mod, bool released) {
		if(! released) {
			switch(keycode) {
				//case SDLK_w:  direction += Direction::FORWARD;   break;
				//case SDLK_s:  direction += Direction::BACKWARD;  break;
				case SDLK_w:  speed_boost = speed * 6.0f;  break;
				case SDLK_s:  speed_boost = speed * -0.7f;  break;
				case SDLK_d:  direction += Direction::RIGHT;     break;
				case SDLK_a:  direction += Direction::LEFT;      break;
			}
		}
	} );

	Transition movement_tr = Transition(
			transition_pos,
			1.0f );

	while(pool::poll_events()) {
		if(direction == Direction::FORWARD)  pos_target[1] -= 1.0f; else
		if(direction == Direction::BACKWARD) pos_target[1] += 1.0f; else
		if(direction == Direction::RIGHT)    pos_target[0] += 1.0f; else
		if(direction == Direction::LEFT)     pos_target[0] -= 1.0f;

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

		frame_timer.reset();
		bool cycle_frame_timer = true;
		float speed_total = speed + speed_boost;
		while(cycle_frame_timer) {
			float delta = speed_total * frame_timer.millis() / 1000.0f;
			frame_timer.reset();
			cycle_frame_timer = movement_tr.advance(delta);

			// delays a bit more than the actual framerate
			std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));

			wobj1->setRotationRad(objrot);
			wobj2->setRotationRad(objrot*2.0f);
			renderer->setView(glm::vec3(-pos[0], -1.0f, -pos[1]), -rot, 0.9f);
			renderer->renderFrame();

			objrot += delta / 10.0f;
			if(objrot >= 6.2830f) objrot -= 6.2830f;
		}
	}

	delete wobj1;
	delete wobj2;
	delete renderer;

	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
