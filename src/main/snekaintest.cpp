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

#define TILES          (25.0)
#define FPS            (30.0)
#define FRAMERATE      (1000.0/FPS)
#define STEPS          (10)
#define FRAMES         (500)



namespace {

	using namespace sneka;


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

	const unsigned char * keyboard;

	GLfloat x=0.0f, z=0.0f;
	bool moving = false;
	Direction direction;

	void update_keyboard() {
		SDL_PumpEvents();
	}

	void (*key_event)(unsigned int keycode, bool released);


	bool poll_events() {
		SDL_Event event;

		static unsigned char lastk_code = 0;
	 	static unsigned char lastk_pressed = 0;

		update_keyboard();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					return false;  break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					update_keyboard();
					if(! (event.key.keysym.sym == lastk_code && event.key.state == lastk_pressed)) {
						lastk_code = event.key.keysym.sym;
						lastk_pressed = event.key.state;
						key_event(lastk_code, lastk_pressed == SDL_RELEASED);
					}
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					std::cout << "tried to resize to " << event.window.data1 << " x " << event.window.data2 << std::endl;
					break;
			}
		}

		return true;
	}

}


int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	const int W = 600, H = 600 * 9 / 16;

	float speed = 0.06f;

	pool::runtime_init(
			"sneka world render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);

	WorldRenderer* renderer = new WorldRenderer(
			"assets/tile.mesh", TILES,
			-0.02f, 0, W, H );
	renderer->clear_color = glm::vec3(0.05f, 0.05f, 0.15f);

	key_event = [](unsigned int keycode, bool released) {
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
	};
	keyboard = SDL_GetKeyboardState(nullptr);

	int cycle = 0;
	while(poll_events()) {
		if(moving) {
			if(direction == Direction::FORWARD)  z -= speed; else
			if(direction == Direction::BACKWARD) z += speed; else
			if(direction == Direction::RIGHT)    x += speed; else
			if(direction == Direction::LEFT)     x -= speed;
		}

		renderer->setView(glm::vec3(-x, -1.0f, -z), ((float) cycle) / 10000.0f, 0.9f);
		renderer->renderFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds((int) FRAMERATE));

		cycle = (cycle + (int) FRAMERATE) % 31415;
	}

	delete renderer;

	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
