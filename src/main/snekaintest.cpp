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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL.h>

#define TILES          (20.0)
#define FPS            (30.0)
#define FRAMERATE      (1000.0/FPS)
#define STEPS          (10)
#define FRAMES         (500)



namespace {

	using namespace sneka;


	/*
	void print_direction(Direction& dir) {
		if(dir == Direction::FORWARD)
			std::cout << "FORWARD" << std::endl;

		if(dir == Direction::LEFT)
			std::cout << "LEFT" << std::endl;

		if(dir == Direction::BACKWARD)
			std::cout << "BACKWARD" << std::endl;

		if(dir == Direction::RIGHT)
			std::cout << "RIGHT" << std::endl;
	}
	*/

	GLfloat x=0.0f, z=0.0f;
	const GLfloat speed = 3.5f / FPS;
	bool moving = false;
	Direction direction;

}


int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	const int W = 600, H = 600 * 9 / 16;

	pool::runtime_init(
			"sneka world render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);

	WorldRenderer* renderer = new WorldRenderer(
			"assets/tile.mesh", TILES,
			-0.03f, 0, W, H );
	renderer->clear_color = glm::vec3(0.05f, 0.05f, 0.15f);

	WorldObject wobj1 = WorldObject("assets/trisquare.mesh");
	wobj1.setGridPosition(1, 0);
	wobj1.setHeight(0.2f);
	renderer->putObject(wobj1);
	WorldObject wobj2 = WorldObject("assets/trisquare.mesh");
	wobj2.setGridPosition(-1, 0);
	wobj2.setHeight(0.2f);
	renderer->putObject(wobj2);

	pool::set_key_callback( [](unsigned int keycode, unsigned int mod, bool released) {
		if(released) {
			moving = false;
		} else {
			moving = true;
			switch(keycode) {
				case SDLK_w:  direction = Direction::FORWARD;   break;
				case SDLK_s:  direction = Direction::BACKWARD;  break;
				case SDLK_d:  direction = Direction::RIGHT;     break;
				case SDLK_a:  direction = Direction::LEFT;      break;
				default: moving = false;
			}
		}
	} );

	int cycle = 0;
	GLfloat objrot = 0.0f;
	while(pool::poll_events()) {
		if(moving) {
			if(direction == Direction::FORWARD)  z -= speed; else
			if(direction == Direction::BACKWARD) z += speed; else
			if(direction == Direction::RIGHT)    x += speed; else
			if(direction == Direction::LEFT)     x -= speed;
		}

		wobj1.setRotationRad(objrot);
		wobj2.setRotationRad(objrot*2.0f);
		renderer->setView(glm::vec3(-x, -1.0f, -z), ((float) cycle) / 5000.0f, 0.9f);
		renderer->renderFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));

		cycle = (cycle + (int) FRAMERATE) % 31415;
		objrot += 0.05f;
		if(objrot > 6.2830f) objrot -= 6.2830f;
	}

	delete renderer;

	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
