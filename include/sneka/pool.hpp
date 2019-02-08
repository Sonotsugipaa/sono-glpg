#ifndef SNEKA_POOL_HPP
#define SNEKA_POOL_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <exception>
#include <string>

#include "runtime.hpp"

#include "sneka/mesh.hpp"

/* Mesh values are stored as integers on file;
 * In order to obtain floats, each integer is divided by
 * a precision factor: POOL_MESH_FILE_PRECISION is that
 * factor. */
#define POOL_MESH_FILE_PRECISION (100.0)



namespace sneka::pool {

	using namespace gla;

	using key_callback_t = void (*) (
			unsigned int keycode,
			unsigned int modifiers,
			bool released );

	extern GLuint
			uniform_proj,
			uniform_view,
			uniform_model,
			uniform_add_col,
			uniform_time,
			uniform_curvature,
			uniform_drugs,

			in_position,
			in_color;
			//in_random;

	const Runtime * runtime();


	void runtime_init(
			std::string window_name, /* title */
			int x, int y,            /* window position: either a positive number,
											  * SDL_WINDOWPOS_CENTERED or
											  * SDL_WINDOWPOS_UNDEFINED */
			int width, int height,
			bool resizable,
			bool vsync );

	void runtime_destroy();

	/* Some render object meshes have dynamic colors, some do not;
	 * this function is called for each object, so that those
	 * without dynamic colors do not use a stray value for the
	 * add_col shader uniform. */
	void set_add_col_enabled(bool);

	/* If need_vertices = true, ensures that the mesh contains
	 * the vertices; if they don't, the file is reloaded and the
	 * mesh is invalidated.
	 * NOTE: doing this might have give significant performance hit. */
	Mesh& get_mesh(std::string name, bool need_vertices = false);

	/* Processes all the events, until none are left to process;
	 * returns 'false' if the application should exit, 'true'
	 * otherwise. */
	bool poll_events();

	/* Returns the state of a key, either 'SDL_PRESSED' or 'SDL_RELEASED.' */
	unsigned char keyboard_state(unsigned int keycode);

	/* Sets a function to be called on key events;
	 * returns the previous callback function, or 'nullptr'
	 * if there was none. */
	key_callback_t set_key_callback(key_callback_t);


	class PoolException : public std::exception {
	protected:
		std::string msg;

	public:
		PoolException(char*);
		PoolException(const char *);
		PoolException(std::string&);
		PoolException(std::string&&);

		const char * what() const noexcept;
	};

}

#endif
