#include <iostream>
#include <cstdio>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "runtime.hpp"
#include "globject.hpp"

#define FPS            (4.0)
#define FRAMES_SECOND  (1000.0/FPS)



namespace gla {

	void render(Runtime* runtime, VertexArray* va_3_4_array, std::size_t size) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(std::size_t i=0; i < size; i+=1) {
			va_3_4_array[i].bind();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		SDL_GL_SwapWindow(runtime->window);
	}

	void run(Runtime* runtime, VertexArray& vao_3_4) {
		SDL_Event event;
		int x=3, y=3;
		bool quit = false;
		bool ignore_ev = false;

		glClearColor(((float) x) / 10.0f, ((float) x) / 10.0f, 0.3f, 1.0f);
		render(runtime, &vao_3_4, 1);

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
						default: ignore_ev = true; break;
					}
					std::cout << x << ", " << y << std::endl;
					break;
				default:
					ignore_ev = true;
			}

			if(! ignore_ev) {
				glClearColor(((float) x) / 10.0f, 0.3f, ((float) y) / 10.0f, 1.0f);

				render(runtime, &vao_3_4, 1);

				glBindVertexArray(0);
				SDL_Delay(FRAMES_SECOND);
			} else {
				ignore_ev = false;
			}
		}
	}


	GLfloat data_vertex[4][3] = {
		{ -0.5f,  0.5f, 0.5f },
		{  0.5f,  0.5f, 0.5f },
		{  0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f }
	};

	GLfloat data_color[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

}


int main(int argn, char** argv) {
	using namespace gla;

	Runtime runtime = Runtime(
		"glatest",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		600, 600,
		true,
		"shader/gltest_v.glsl",
		"shader/gltest_f.glsl" );

	VertexBuffer vb_vertex = VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	VertexBuffer vb_color =  VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vb_vertex.bufferData(data_vertex, 4*3*sizeof(GLfloat));
	vb_color.bufferData(data_color, 4*4*sizeof(GLfloat));

	VertexArray va;
	va.assignVertexBuffer(
			vb_vertex,
			glGetAttribLocation(runtime.shader->program, "in_position"),
			3,
			GL_FLOAT, GL_FALSE,
			0, 0);
	va.assignVertexBuffer(
			vb_color,
			glGetAttribLocation(runtime.shader->program, "in_color"),
			4,
			GL_FLOAT, GL_FALSE,
			0, 0);

	run(&runtime, va);

	return EXIT_SUCCESS;
}
