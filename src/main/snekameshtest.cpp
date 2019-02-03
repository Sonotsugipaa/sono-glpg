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

	GLuint uniform_proj;
	GLuint uniform_trans;

	glm::mat4 mat_proj;
	glm::mat4 mat_trans;


	/*
	void read_int_to(FILE* file, GLfloat* dest, GLfloat mul) {
		int i=0;
		fread(&i, sizeof(int), 1, file);
		*dest = (GLfloat) i / mul;
	}

	sneka::Mesh load_mesh(const char * path) {
		using namespace sneka;

		Vertex* vertices;
		std::size_t len;

		FILE* f = fopen(path, "rb");
		fseek(f, 0, SEEK_END);
		len = ftell(f) / (sizeof(int) * 7);
		fseek(f, 0, SEEK_SET);

		vertices = new Vertex[len];

		for(std::size_t i=0; i<len; i+=1) {
			read_int_to(f, &vertices[i].position_color[0], 10.0f);
			read_int_to(f, &vertices[i].position_color[1], 10.0f);
			read_int_to(f, &vertices[i].position_color[2], 10.0f);
			read_int_to(f, &vertices[i].position_color[3], 255.0f);
			read_int_to(f, &vertices[i].position_color[4], 255.0f);
			read_int_to(f, &vertices[i].position_color[5], 255.0f);
			read_int_to(f, &vertices[i].position_color[6], 255.0f);
		}
		fclose(f);

		Mesh retn = Mesh(vertices, len);
		delete[] vertices;
		return retn;
	}
	*/

	void draw_square(gla::VertexArray& va) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec4 o = mat_trans * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		va.bind();
		//glDrawArrays(GL_TRIANGLES, 0, squareMesh.size());
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		SDL_GL_SwapWindow(sneka::pool::runtime()->window);
	}

	void set_pos(glm::vec3 pos, GLfloat rot) {
		mat_trans = glm::rotate(
				glm::translate(glm::mat4(1.0f), pos),
				rot,
				glm::vec3(0.0f, 1.0f, 0.0f) );

		glUniformMatrix4fv(
				sneka::pool::uniform_trans, /* uniform location   */
				1,             /* number of matrices */
				GL_FALSE,      /* transpose          */
				&mat_trans[0][0]   /* first value        */ );
	}

	void draw_thread(GLfloat* rotation, bool* quit) {
		*rotation = 0.0f;

		while(! *quit) {
			std::this_thread::sleep_for(std::chrono::milliseconds(16));

			*rotation += 0.01f;

			//std::cout << "rotation " << rotation << std::endl;
		}
	}


	void run(gla::VertexArray& va) {
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

		draw_square(va);

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

				draw_square(va);

				SDL_Delay(FRAMES_SECOND);
			} else {
				ignore_ev = false;
			}
		}

		drawer.join();
	}

}

GLfloat vals[4][8] = {
	{ -1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  0.1465f },
	{  1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  0.4671f },
	{  1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.1496f },
	{ -1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  0.7626f }
};


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

	const Runtime * runtime = pool::runtime();
	VertexBuffer vb = VertexBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	vb.bufferData(vals, 4 * 8 * sizeof(GLfloat));
	VertexArray va = VertexArray();

	va.assignVertexBuffer(
			vb,
			glGetAttribLocation(runtime->shader->program, "in_position"),
			3, GL_FLOAT, GL_FALSE,
			8 * sizeof(GLfloat), (GLfloat*) (0 * sizeof(GLfloat)) );
	va.assignVertexBuffer(
			vb,
			glGetAttribLocation(runtime->shader->program, "in_color"),
			4, GL_FLOAT, GL_FALSE,
			8 * sizeof(GLfloat), (GLfloat*) (3 * sizeof(GLfloat)) );
	va.assignVertexBuffer(
			vb,
			glGetAttribLocation(runtime->shader->program, "in_random"),
			1, GL_FLOAT, GL_FALSE,
			8 * sizeof(GLfloat), (GLfloat*) (7 * sizeof(GLfloat)) );

	run(va);

	//quitGl();
	pool::runtime_destroy();
	return EXIT_SUCCESS;
}
