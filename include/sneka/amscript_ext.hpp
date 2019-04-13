#ifndef SNEKA_AMSCRIPT_EXT_HPP
#define SNEKA_AMSCRIPT_EXT_HPP

#include "amscript/amscript.hpp"



namespace sneka {

	/* Loads an amscript from a file, importing in it
	 * sneka built-in references. */
	amscript::Amscript load_amscript(std::istream& input_stream);
	amscript::Amscript load_amscript(std::string input);	

}

#endif
