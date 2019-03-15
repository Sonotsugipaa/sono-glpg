#ifndef SNEKA_SNEKARUNTIME_HPP
#define SNEKA_SNEKARUNTIME_HPP

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <exception>
#include <string>

#include <glm/mat4x4.hpp>

#include "runtime.hpp"
#include "shader.hpp"
#include "objcounter.tpp"

#include "sneka/mesh.hpp"

/* Mesh values are stored as integers on file;
 * In order to obtain floats, each integer is divided by
 * a precision factor: POOL_MESH_FILE_PRECISION is that
 * factor. */
#define POOL_MESH_FILE_PRECISION (100.0)



namespace sneka {

	// not polymorphic
	class SnekaRuntime : public gla::Runtime {
	public: // typedefs only
		using key_callback_t = void (*) (
				SnekaRuntime& runtime,
				unsigned int keycode,
				unsigned int modifiers,
				bool released );

		using resize_callback_t = void (*) (
				SnekaRuntime& runtime,
				unsigned int width,
				unsigned int height );

	private:
		key_callback_t key_cb;
		resize_callback_t rsz_cb;

		const unsigned char * keyboard_state;

		GLuint vp_w, vp_h;
		GLfloat vp_r;
		GLuint64 vp_s; // viewport_state;

	protected:
		gla::ObjectCounter<> runtime_instances;
		mutable gla::ShaderProgram shader_world, shader_hud;

	public:
		SnekaRuntime(
				std::string window_name, /* title */
				int x, int y,            /* main window position:
				                          * either a positive number,
												  * SDL_WINDOWPOS_CENTERED or
												  * SDL_WINDOWPOS_UNDEFINED */
				int width, int height,
				bool resizable,
				bool vsync );

		~SnekaRuntime();


		GLuint getViewportWidth() const;
		GLuint getViewportHeight() const;
		GLfloat getViewportRatio() const;

		/* With two invocations, returns the same value if and only if
		 * the viewport hasn't been changed
		 * (with a chance of less than a 1/18446744073709551616th of
		 * returning the same value even if the viewport has been
		 * changed - TL;DR, use this for cached values only). */
		GLuint64 getViewportState() const;

		void setViewport(GLuint width, GLuint height);

		gla::ShaderProgram& getWorldShader() const;
		gla::ShaderProgram& getHudShader() const;

		/* If need_vertices = true, ensures that the mesh contains
		 * the vertices; if they don't, the file is reloaded and the
		 * mesh is invalidated.
		 * NOTE: doing this might give a significant performance hit. */
		Mesh& getMesh(std::string name, bool need_vertices = false) const;

		/* Processes all the events, until none are left to process;
		 * returns 'false' if the application should exit, 'true'
		 * otherwise. */
		bool pollEvents();

		/* Returns the state of a key, either 'SDL_PRESSED' or 'SDL_RELEASED.' */
		unsigned char getKeyboardState(unsigned int keycode) const;

		/* Sets a function to be called on key events;
		 * returns the previous callback function, or 'nullptr'
		 * if there was none. */
		key_callback_t setKeyCallback(key_callback_t);

		/* Sets a function to be called on resize events;
		 * returns the previous callback function, or 'nullptr'
		 * if there was none. */
		resize_callback_t setResizeCallback(resize_callback_t);
	};

	using key_callback_t = SnekaRuntime::key_callback_t;
	using resize_callback_t = SnekaRuntime::resize_callback_t;


	class SnekaRuntimeException : public std::exception {
	public:
		const SnekaRuntime & runtime;
		const std::string msg;

		SnekaRuntimeException(const SnekaRuntime & runtime, char*);
		SnekaRuntimeException(const SnekaRuntime & runtime, const char *);
		SnekaRuntimeException(const SnekaRuntime & runtime, const std::string &);
		SnekaRuntimeException(const SnekaRuntime & runtime, const std::string &&);

		const char * what() const noexcept;
	};

}

#endif
