#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <exception>

using std::size_t;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
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

			map["?"] = "?";
			map["!"] = "!";

			args += 1;

			while((arg = *(args++)) != nullptr) {
				if(arg[0] == '!') {
					const char * arg_next = *(args++);
					if(arg_next == nullptr)
						break;

					if(arg_next[0] == '[') {
						stringstream strs;

						// remaining characters in 'arg_next'
						strs << arg_next+1 << ' ';

						while((arg_next = *(args++)) != nullptr) {
							strs << arg_next << ' ';

							strs.seekg(-2, strs.end); // -2 because the last char is a whitespace
							if(strs.peek() == ']') {
								/*
								strs.seekp(-2, strs.end);
								strs << '\0' << '\0';
								*/
								//cout << "\033[1;34m"<<c<<"\033[m" << endl;
								break;
							}
						}
						if(arg_next == nullptr) args -= 1;

						/* this must be terribly inefficient, since two copies
						 * of the buffer are created;
						 * thank the STL sucking so much. */
						string put = strs.str();
						put.resize(put.size() - 2);
						map[arg+1] = put;
					} else {
						map[arg+1] = arg_next;
					}
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
