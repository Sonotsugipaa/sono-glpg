#ifndef SNEKA_POOL_HPP
#define SNEKA_POOL_HPP

#include <exception>
#include <string>

#include "runtime.hpp"



namespace sneka::pool {

	using namespace gla;

	extern GLuint
			uniform_proj,
			uniform_trans,
			uniform_add_col,
			uniform_time,
			uniform_curvature,
			uniform_drugs;

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


	class PoolException : public std::exception {
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
