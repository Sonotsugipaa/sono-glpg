#include <iostream>
#include <cstdio>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <SDL2/SDL.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "runtime.hpp"
#include "globject.hpp"

#define FPS            (8.0)
#define FRAMES_SECOND  (1000.0/FPS)
#define STEPS          (10)



namespace gla {

	GLint uniform_trans, uniform_proj;


	class Shape {
	protected:
		VertexArray va;
		glm::mat4 trans;
		glm::vec2* vertices_cache = nullptr;
		GLuint vectors_n;

		void cache_reset() {
			if(vertices_cache != nullptr)
				delete[] vertices_cache;
		}

		void cache_populate() {
			if(vertices_cache == nullptr)
				vertices_cache = new glm::vec2[vectors_n];

			for(GLuint i=0; i < vectors_n; i+=1) {
				//std::cout << "populated " << va.id() << "[" << i << "]" << std::endl;
				vertices_cache[i] = glm::vec2(trans * glm::vec4(0.0f));
				std::cout << "trans " << vertices_cache[i].x << ", " << vertices_cache[i].y << std::endl;
			}
		}
	public:
		Shape(
				VertexBuffer& vertex_vb, VertexBuffer& color_vb,
				std::size_t vectors_n
		):
				vectors_n(vectors_n)
		{
			va.assignVertexBuffer(
					vertex_vb, 0,
					3, GL_FLOAT, GL_FALSE,
					0, 0 );
			va.assignVertexBuffer(
					color_vb, 1,
					4, GL_FLOAT, GL_FALSE,
					0, 0 );
			cache_populate();
		}

		void draw() {
			va.bind();
			glUniformMatrix4fv(
					uniform_trans, /* uniform location   */
					1,             /* number of matrices */
					GL_FALSE,      /* transpose          */
					&trans[0][0]   /* first value        */ );
			glDrawArrays(GL_LINE_LOOP, 0, vectors_n);
		}

		void setPos(glm::vec3 pos, float rotation) {
			std::cout << "set pos " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			/*
			trans = glm::rotate(
					glm::translate(glm::mat4(1.0f), pos),
					rotation,
					glm::vec3(0.0f, 0.0f, 1.0f) );
			*/
			trans = glm::translate(glm::mat4(1.0f), pos);
			cache_populate();
		}
	};


	void render(Runtime* runtime, Shape* shape, std::size_t size) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(std::size_t i=0; i < size; i+=1) {
			shape[i].draw();
		}

		SDL_GL_SwapWindow(runtime->window);
	}

	void run(Runtime* runtime, Shape* shape) {
		SDL_Event event;
		int x=0, y=0, z=0;
		bool quit = false;
		bool ignore_ev = false;

		glClearColor(
				((float) (x + (STEPS/2))) / STEPS,
				((float) (y + (STEPS/2))) / STEPS,
				((float) (z + (STEPS/2))) / STEPS,
				1.0f );

		shape->setPos(
				glm::vec3(
					( (float) x) / STEPS,
					( (float) y) / STEPS,
					(((float) z) / STEPS) -1.0f ),
				0 );

		render(runtime, shape, 1);

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
				glClearColor(
						((float) (x + (STEPS/2))) / STEPS,
						((float) (y + (STEPS/2))) / STEPS,
						((float) (z + (STEPS/2))) / STEPS,
						1.0f );

				shape->setPos(
						glm::vec3(
							( (float) x) / STEPS,
							( (float) y) / STEPS,
							(((float) z) / STEPS) -1.0f ),
						0 );
				render(runtime, shape, 1);

				SDL_Delay(FRAMES_SECOND);
			} else {
				ignore_ev = false;
			}
		}
	}


	GLfloat data_vertex[5][3] = {
		{ -0.5f,  0.5f, 0.0f },
		{  0.0f, 0.7f, 0.0f },
		{  0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f }
	};

	GLfloat data_color[5][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

}


int main(int argn, char** argv) {
	using namespace gla;

	const GLint
		WIDTH = 600,
		HEIGHT = 500;

	Runtime runtime = Runtime(
		"glatest",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT,
		true, true,
		"shader/gltest_v.glsl",
		"shader/gltest_f.glsl" );

	uniform_trans = glGetUniformLocation(runtime.shader->program, "uni_trans");
	uniform_proj = glGetUniformLocation(runtime.shader->program, "uni_proj");
	std::cout << "uni_trans " << uniform_trans << std::endl;
	std::cout << "uni_proj  " << uniform_proj << std::endl;
	{
		glm::mat4 proj = glm::perspective(90.0f, (GLfloat) WIDTH / HEIGHT, 0.2f, 100.0f);
		glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, &proj[0][0]);
	}

	VertexBuffer vb_vertex = VertexBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	VertexBuffer vb_color =  VertexBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	vb_vertex.bufferData(data_vertex, 5*3*sizeof(GLfloat));
	vb_color.bufferData(data_color, 5*4*sizeof(GLfloat));

	std::cout
			<< "pos attrib " << glGetAttribLocation(runtime.shader->program, "in_position") << std::endl
			<< "col attrib " << glGetAttribLocation(runtime.shader->program, "in_color") << std::endl;

	Shape shape = Shape(vb_vertex, vb_color, 5);

	run(&runtime, &shape);

	return EXIT_SUCCESS;
}
