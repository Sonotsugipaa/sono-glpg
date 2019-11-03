#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace sneka;



inline namespace test {

	const string SUCCESS = "\033[1;94mSUCCESS\033[m";
	const string FAILURE = "\033[1;91mFAILURE\033[m";
	const string PEDESTAL = "\033[1m #-------#  \033[m";


	template<typename arg_t>
	inline bool run(
			string description,
			bool (*test_function)(arg_t),
			arg_t arg = static_cast<nullptr_t>(nullptr)
	) {
		bool r = test_function(std::move(arg));
		cout << PEDESTAL << description << ": " << (r? SUCCESS : FAILURE) << endl;
		return r;
	}

	inline bool run(
			string description,
			bool (*test_function)()
	) {
		bool r = test_function();
		cout << PEDESTAL << description << ": " << (r? SUCCESS : FAILURE) << endl;
		return r;
	}

}

#endif
