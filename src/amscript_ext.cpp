#include "sneka/amscript_ext.hpp"

#include "utils.tpp"

#include <chrono>

/*
#pragma GCC warning "#include <iostream>"
#include <iostream>
*/

using namespace amscript2;
using amscript2::int_t;
using amscript2::ref_t;



namespace {

	void set_random(int_t rnd, ScopeView& scope) {
		scope.base()->defineGlobal(
			ref_t("random"),
			Definition::ptr(new ArrayDefinition(rnd))
		);
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

	std::vector<Value> ext_random_choose(std::vector<Value> args, ScopeView& scope) {
		std::vector<Value> retn;

		if(! args.empty()) {
			std::vector<Value> random_symbol_val =
				scope.resolve(ref_t("random"))->evaluate({ }, scope);
			if(random_symbol_val.empty())  random_symbol_val = { (int_t) 0 };
			unsigned int random =
					gla::xorshift(
						random_symbol_val[0].intValue()
					);
			retn.push_back(std::move(args[random % args.size()]));
		}

		// But... how? Talk to me, old-myself, talk to me god damnit!
		throw std::string("random_choose is broken");

		return retn;
	}

	std::vector<Value> ext_random_set(std::vector<Value> args, ScopeView& scope) {
		if(args.empty()) {
			set_random(
					std::chrono::system_clock::now().time_since_epoch().count(),
					scope
			);
		} else {
			unsigned int seed = 0;
			for(std::size_t i = 0; i < args.size(); i+=1)
				seed ^= xorshift_str(args[i].strValue());
			set_random(seed, scope);
		}

		throw std::string("random_set is broken");
		return { };
	}

	std::vector<Value> ext_random_number(std::vector<Value>) {
		throw std::string("random_number not implemented yet");
	}

	const Script base_script = []() {
		Script r;
		r.define(ref_t("random_set"), ext_random_set);
		r.define(ref_t("random_choose"), ext_random_choose);
		r.define(ref_t("random_number"), ext_random_number);
		return r;
	} ();

}


namespace sneka {

	Script load_amscript(std::istream& istr) {
		Script clone = base_script;
		clone.import(Script(istr));
		return clone;
	}

	Script load_amscript(std::string in) {
		Script clone = base_script;
		clone.import(Script(std::move(in)));
		return clone;
	}

}
