#ifndef SNEKA_AMSCRIPT_EXT_HPP
#define SNEKA_AMSCRIPT_EXT_HPP

#include "amscript/amscript2.hpp"



namespace sneka {

	/* Loads an amscript from a file, importing in it
	 * sneka built-in references. */
	amscript2::Script load_amscript(std::istream& input_stream);
	amscript2::Script load_amscript(std::string input);	

}

#endif
