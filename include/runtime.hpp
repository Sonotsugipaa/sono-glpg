#ifndef SONOGLPG_RUNTIME_HPP
#define SONOGLPG_RUNTIME_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "shader.hpp"



namespace gla {

	class Runtime {
	public:
		SDL_Window * window;
		SDL_GLContext * context;
		ShaderProgram * shader;

		Runtime(
				std::string window_name, /* title */
				int x, int y,            /* window position: either a positive number,
				                          * SDL_WINDOWPOS_CENTERED or
				                          * SDL_WINDOWPOS_UNDEFINED */
				int width, int height,
				bool resizable,
				bool vsync,
				std::string vertex_shader_name,
				std::string fragment_shader_name );

		~Runtime();
	};

}

#endif
