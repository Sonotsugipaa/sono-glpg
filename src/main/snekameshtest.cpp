#include <iostream>
#include <thread>
#include <chrono>

#include <cstdlib>
#include <cstdio>

#include "sneka/pool.hpp"

#include "runtime.hpp"
#include "shader.hpp"
#include "globject.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <SDL2/SDL.h>

#define FPS            (8.0)
#define FRAMES_SECOND  (1000.0/FPS)
#define STEPS          (10)



namespace {

	//gla::Runtime* runtime;

	glm::mat4 mat_proj;
	glm::mat4 mat_view;


	void draw_square(sneka::Mesh& mesh) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.draw();

		SDL_GL_SwapWindow(sneka::pool::runtime()->window);
	}

	void set_pos(glm::vec3 pos, GLfloat rot) {
		mat_view = glm::rotate(
				glm::translate(glm::mat4(1.0f), pos),
				rot,
				glm::vec3(0.0f, 1.0f, 0.0f) );

		glUniformMatrix4fv(
				sneka::pool::uniform_view, /* uniform location   */
				1,             /* number of matrices */
				GL_FALSE,      /* transpose          */
				&mat_view[0][0]   /* first value        */ );
	}

	void draw_thread(GLfloat* rotation, bool* quit) {
		*rotation = 0.0f;

		while(! *quit) {
			std::this_thread::sleep_for(std::chrono::milliseconds(16));

			*rotation += 0.01f;

			//std::cout << "rotation " << rotation << std::endl;
		}
	}


	void run(sneka::Mesh& mesh) {
		SDL_Event event;
		int x=0, y=0, z=0;
		bool quit = false;
		bool ignore_ev = false;
		GLfloat rotation = 0.0f;

		std::thread drawer = std::thread(draw_thread, &rotation, &quit);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		set_pos(
				glm::vec3(
					( (GLfloat) x) / STEPS,
					( (GLfloat) y) / STEPS,
					(((GLfloat) z) / STEPS) -1.0f ),
				0.0f );

		draw_square(mesh);

		while(! quit) {
			SDL_PollEvent(&event);

			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT:      x--;  break;
						case SDLK_RIGHT:     x++;  break;
						case SDLK_UP:        y--;  break;
						case SDLK_DOWN:      y++;  break;
						case SDLK_PAGEUP:    z--;  break;
						case SDLK_PAGEDOWN:  z++;  break;
						default: ignore_ev = true; break;
					}
					//std::cout << x << ", " << y << std::endl;
					break;
				default:
					ignore_ev = true;
			}

			if(! ignore_ev) {
				//std::cout << "x" << x << ", y" << y << ", z" << z << std::endl;
				set_pos(
						glm::vec3(
							( (GLfloat) x) / STEPS,
							( (GLfloat) y) / STEPS,
							(((GLfloat) z) / STEPS) -1.0f ),
						rotation );

				draw_square(mesh);

				SDL_Delay(FRAMES_SECOND);
			} else {
				ignore_ev = false;
			}
		}

		drawer.join();
	}

}


int main(int argn, char** args) {
	using namespace sneka;
	using namespace gla;

	const int W = 600, H = 400;

	//initGl();
	pool::runtime_init(
			"sneka mesh test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			W, H, true, true);

	mat_proj = glm::perspective(90.0f, (GLfloat) W / H, 0.2f, 100.0f);
	glUniformMatrix4fv(pool::uniform_proj, 1, GL_FALSE, &mat_proj[0][0]);

	Mesh& square_mesh = pool::get_mesh("assets/trisquare.mesh");
	pool::get_mesh("assets/trisquare.mesh");

	// test if an invalid mesh triggers an exception
	try {
		pool::get_mesh("makefile");
	} catch (pool::PoolException ex) {
		std::cout
				<< std::endl
				<< "intentional exception triggered: " << ex.what() << std::endl
				<< "NOTE: this is part of the unit test, do not worry about this." << std::endl
				<< std::endl;
	}

	run(square_mesh);

	//quitGl();
	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
