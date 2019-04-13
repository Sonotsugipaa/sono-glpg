#include "sneka/amscript_ext.hpp"

#include "utils.tpp"

#include <chrono>

/*
#pragma GCC warning "#include <iostream>"
#include <iostream>
*/

using namespace amscript;
using strvec = std::vector<std::string>;



namespace {

	void set_random(Amscript* scr, std::string arg) {
		scr->setSymbol("$random", amscript::Symbol(arg, false));
	}

	unsigned int xorshift_str(const std::string & src) {
		unsigned int rnd = 0;

		for(std::size_t i = 0; i < src.size(); i+=1) {
			constexpr const unsigned int byte_mask = ~((unsigned char) 0);
			rnd ^=
					(static_cast<unsigned int>(src[i]) & byte_mask) <<
					((i*8) % sizeof(unsigned int));
		}

		return rnd;
	}

	strvec ext_random_choose(Amscript* scr, strvec args) {
		strvec retn;

		if(! args.empty()) {
			unsigned int random =
					gla::xorshift(
						xorshift_str(scr->resolveSymbol("$random"))
					);
			retn.push_back(args[random % args.size()]);
		}

		// But... how? Talk to me, old-myself, talk to me god damnit!
		throw amscript::RuntimeException("random_choose is broken");

		return retn;
	}

	strvec ext_random_set(Amscript* scr, strvec args) {
		if(args.empty()) {
			set_random(
					scr,
					std::to_string(
						std::chrono::system_clock::now().time_since_epoch().count()
					) );
		} else {
			unsigned int seed = 0;
			for(std::size_t i = 0; i < args.size(); i+=1)
				seed ^= xorshift_str(args[i]);
			set_random(scr, std::to_string(seed));
		}

		throw amscript::RuntimeException("random_set is broken");
		return strvec();
	}

	strvec ext_random_number(Amscript* scr, strvec args) {
		(void) scr;
		(void) args;
		throw amscript::RuntimeException("random_number not implemented yet");
	}

	inline void overload(amscript::Amscript& amscript) {
		amscript.setFunction("$random_set", ext_random_set);
		amscript.setFunction("$random_choose", ext_random_choose);
		amscript.setFunction("$random_number", 2, ext_random_number);
	}

}


namespace sneka {

	Amscript load_amscript(std::istream& istr) {
		Amscript scr = Amscript(istr);
		overload(scr);
		return scr;
	}

	Amscript load_amscript(std::string in) {
		Amscript scr = Amscript(std::move(in));
		overload(scr);
		return scr;
	}

}
