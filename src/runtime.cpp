#include "runtime.hpp"

/* SHOULD BE REMOVED, ONCE YOU FIGURE *
 * OUT HOW TO PROPERLY USE EXCEPTIONS */
#include <iostream>



namespace gla {

	Runtime::Runtime(
			std::string name,
			int x, int y, int w, int h,
			bool resizable, bool vsync,
			std::string vshader, std::string fshader
	) {
		// SDL
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout <<
					"\033[1;91mCould not initialize SDL.\n"
					"(TODO: USE EXCEPTIONS INSTEAD OF EXIT() )\033[m\n";
			exit(EXIT_FAILURE);
		}

		// SDL Window
		window = SDL_CreateWindow(
			name.c_str(),
			x, y, w, h,
			SDL_WINDOW_OPENGL | (resizable? SDL_WINDOW_RESIZABLE : 0) );

		// SDL Context
		context = new SDL_GLContext();
		*context = SDL_GL_CreateContext(window);

		// GL Attribs
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// VSync, apparently...?
		if(vsync)
			SDL_GL_SetSwapInterval(1);

		if(resizable) {
			SDL_SetWindowResizable(window, SDL_TRUE);
		} else {
			SDL_SetWindowResizable(window, SDL_FALSE);
		}


		// GLEW
		glewExperimental = GL_TRUE;
		glewInit();

		// GL Shader
		shader = new ShaderProgram(vshader, fshader);
		shader->use();
	}

	Runtime::~Runtime() {
		SDL_GL_DeleteContext(*context);
		SDL_DestroyWindow(window);
		delete context;
		delete shader;
		SDL_Quit();
	}

}
