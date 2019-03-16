#include "sneka/levelfile.hpp"

#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using namespace sneka;



int main(int argn, char** args) {
	if(argn < 2)
		return EXIT_FAILURE;

	std::ifstream* is = new std::ifstream(args[1]);
	LevelFile file = LevelFile(*is);
	delete is;

	cout << "type  = '" << (std::string) file["type"] << '\'' << endl;
	cout << "itype = '" << (int) file["type"] << '\'' << endl;
	cout << "ftype = '" << (float) file["type"] << '\'' << endl;
	cout << "invalid = '" << (std::string) file["invalid"] << '\'' << endl;

	return EXIT_SUCCESS;
}
