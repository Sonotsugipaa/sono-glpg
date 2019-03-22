#ifndef SONOGLPG_EXCEPTION_HPP
#define SONOGLPG_EXCEPTION_HPP

#include <exception>



namespace {

	class GlException : public std::exception {
		std::string msg;
	public:
		GlException(const std::string & message): msg(message) { }
		GlException(const char * message): msg(message) { }
		inline const char * what() const noexcept { return msg.c_str(); }
	};

}

#endif
