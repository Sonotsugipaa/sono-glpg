#include <iostream>
#include <cstdio>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <SDL2/SDL.h>

#define FPS            (4.0)
#define FRAMES_SECOND  (1000.0/FPS)



namespace gla {

	struct Runtime {
		SDL_Window* window;
		SDL_GLContext context;
	};

	Runtime* runtime;


	void init() {
		runtime = new Runtime();

		// SDL
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "\033[1;91mCould not initialize SDL.\033[m\n";
			exit(EXIT_FAILURE);
		}

		// SDL Window
		runtime->window = SDL_CreateWindow(
			"My SDL Empty Window",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			640, 480,
			SDL_WINDOW_OPENGL );

		// SDL Context
		runtime->context = SDL_GL_CreateContext(runtime->window);

		// GL Attribs
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// VSync, apparently...?
		SDL_GL_SetSwapInterval(1);

		// GLEW
		glewExperimental = GL_TRUE;
		glewInit();
	}

	void cleanup() {
		SDL_GL_DeleteContext(runtime->context);
		SDL_DestroyWindow(runtime->window);
		delete runtime;
		SDL_Quit();
	}

	void render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(gla::runtime->window);
	}

	void run() {
		SDL_Event event;
		int x=3, y=3;
		bool quit = false;
		bool ignore_ev = false;

		glClearColor(((float) x) / 10.0f, ((float) x) / 10.0f, 0.3f, 1.0f);
		render();

		while(! quit) {
			SDL_PollEvent(&event);

			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT:  x--; break;
						case SDLK_RIGHT: x++; break;
						case SDLK_UP:    y--; break;
						case SDLK_DOWN:  y++; break;
					}
					std::cout << x << ", " << y << std::endl;
					break;
				default:
					ignore_ev = true;
			}

			if(! ignore_ev) {
				glClearColor(((float) x) / 10.0f, 0.3f, ((float) y) / 10.0f, 1.0f);
				render();
				SDL_Delay(FRAMES_SECOND);
			} else {
				ignore_ev = false;
			}
		}
	}

}


int main (int argn, char** argv) {
	gla::init();

	gla::run();

	gla::cleanup();
	return EXIT_SUCCESS;
}
