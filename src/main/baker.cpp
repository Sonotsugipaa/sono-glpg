#include <iostream>
#include <string>
#include <map>
#include <exception>

using std::size_t;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::pair;



namespace {
	using str_map = std::map<string, string>;

	/*
	const char * seek(const char * ptr) {
		char c;
		while((c=*ptr) != '\0' && c != '=') {
			ptr += 1;
		}
		return ptr;
	}
	*/

	void error(const string msg, bool fatal = true) {
		cerr << "\033[1;91mError: " << msg << "\033[m" << endl;
		if(fatal) exit(EXIT_FAILURE);
	}
}


#ifndef _POSIX_C_SOURCE

	#error "This compiler is not POSIX-compliant."

#else // #ifndef _POSIX_C_SOURCE

	int main(int argn, char** args) {
		try {
			str_map map;
			const char * arg;

			map["br"] = "\n";
			map["?"] = "?";
			map["!"] = "!";

			args += 1;

			while((arg = *(args++)) != nullptr) {
				if(arg[0] == '!') {
					const char * arg_next = *(args++);
					if(arg_next == nullptr)
						break;
					map[arg+1] = arg_next;
				} else
				if(arg[0] == '?') {
					auto found = map.find(string(arg+1));
					if(found != map.end())
						cout << found->second << ' ';
				} else {
					cout << arg << ' ';
				}
			}

			cout << endl;
		} catch(std::exception& ex) {
			error(string(ex.what()), false);
			return EXIT_FAILURE;
		} catch(...) {
			error(string("unknown exception"), false);
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

#endif // #ifndef _POSIX_C_SOURCE
