#include <iostream>
#include <cstdio>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "shader.hpp"

#define FPS            (4.0)
#define FRAMES_SECOND  (1000.0/FPS)



namespace gla {

	struct Runtime {
		SDL_Window* window;
		SDL_GLContext context;
		ShaderProgram* shader;
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

		// GL Shader
		runtime->shader = new ShaderProgram("shader/gltest_v.glsl", "shader/gltest_f.glsl");
		runtime->shader->use();
	}

	void cleanup() {
		SDL_GL_DeleteContext(runtime->context);
		SDL_DestroyWindow(runtime->window);
		delete runtime->shader;
		delete runtime;
		SDL_Quit();
	}

	void render(GLuint* vao_array, std::size_t size) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(std::size_t i=0; i < size; i+=1) {
			glBindVertexArray(vao_array[i]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		SDL_GL_SwapWindow(gla::runtime->window);
	}

	GLuint squareVertexVbo() {
		static const int points=4, coords=3;
		static const GLfloat square[points][coords] = {
			{ -0.5f,  0.5f, 0.5f },
			{  0.5f,  0.5f, 0.5f },
			{  0.5f, -0.5f, 0.5f },
			{ -0.5f, -0.5f, 0.5f }
		};
		static GLuint retn = 0;

		if(retn == 0) {
			glGenBuffers(1, &retn);
			glBindBuffer(GL_ARRAY_BUFFER, retn);
			glBufferData(
					GL_ARRAY_BUFFER,
					points * coords * sizeof(GLfloat),
					square,
					GL_STATIC_DRAW );
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			std::cout << "vtx vbo = " << retn << std::endl;
		}

		return retn;
	}

	GLuint squareColorVbo() {
		static const int points=4, coords=4;
		static const GLfloat square[points][coords] = {
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f }
		};
		static GLuint retn = 0;

		if(retn == 0) {
			glGenBuffers(1, &retn);
			glBindBuffer(GL_ARRAY_BUFFER, retn);
			glBufferData(
					GL_ARRAY_BUFFER,
					points * coords * sizeof(GLfloat),
					square,
					GL_STATIC_DRAW );
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			std::cout << "col vbo = " << retn << std::endl;
		}

		return retn;
	}

	GLuint squareVao() {
		static GLuint retn = 0;

		if(retn == 0) {
			GLuint attrib;

			attrib = glGetAttribLocation(runtime->shader->program, "in_position");
			glGenVertexArrays(1, &retn);
			glBindVertexArray(retn);
			
			glBindBuffer(GL_ARRAY_BUFFER, squareVertexVbo());
			glVertexAttribPointer(
					attrib,
					3,        /* number of values */
					GL_FLOAT, /* type of values   */
					GL_FALSE, /* normalized       */
					0,        /* stride           */
					0         /* offset           */ );
			glEnableVertexAttribArray(attrib);

			attrib = glGetAttribLocation(runtime->shader->program, "in_color");
			glBindBuffer(GL_ARRAY_BUFFER, squareColorVbo());
			glVertexAttribPointer(
					attrib,
					4,        /* number of values */
					GL_FLOAT, /* type of values   */
					GL_FALSE, /* normalized       */
					0,        /* stride           */
					0         /* offset           */ );

			glEnableVertexAttribArray(attrib);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			std::cout << "vao = " << retn << std::endl;
		}

		return retn;
	}

	void run() {
		SDL_Event event;
		int x=3, y=3;
		bool quit = false;
		bool ignore_ev = false;
		GLuint vao = squareVao();


		glClearColor(((float) x) / 10.0f, ((float) x) / 10.0f, 0.3f, 1.0f);
		render(&vao, 1);

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

				render(&vao, 1);

				glBindVertexArray(0);
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
