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



int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	const int W = 600, H = 600 * 9 / 16;

	pool::runtime_init(
			"sneka world render test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);

	WorldRenderer renderer = WorldRenderer(
			"assets/tile.mesh", TILES,
			-0.02f, 0, W, H );
	renderer.clear_color = glm::vec3(0.05f, 0.05f, 0.15f);

	GLfloat rot = 0.0f;

	GridObject wobj1 = GridObject("assets/trisquare.mesh");
	wobj1.setGridPosition(0, 0);
	wobj1.setHeight(0.5f);
	renderer.putObject(wobj1);
	GridObject wobj2 = GridObject("assets/tile.mesh");
	wobj2.setGridPosition(1, 0);
	wobj2.setHeight(0.5f);
	renderer.putObject(wobj2);

	glm::vec3 pos = glm::vec3(0.0f, -1.0f, -10.0f);

	for(int i=0; i < FRAMES; i+=1) {
		pos[2] += (GLfloat) FRAMERATE / FRAMES;
		if(pos[2] > ((float) TILES / 2.0f))
			pos[2] -= (float) TILES;

		renderer.setView(pos, rot, 0.9f);
		wobj1.setRotationRad(rot);
		wobj2.setRotationRad(0.1f - (rot * 0.7f));
		rot += 0.03f;

		renderer.renderFrame();

		SDL_Delay(FRAMERATE);
	}

	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
