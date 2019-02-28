#include <cstdio>
#include <cstdlib>

#include "read_utils.hpp"



int main(int argn, char** args) {
	int read;
	bool error = false;

	args += 1;
	if(argn == 1) return EXIT_SUCCESS;

	while(*args != NULL) {
		error |= ! read_dec_int(*args, &read);
		fwrite(&read, sizeof(int), 1, stdout);

		args += 1;
	}

	return error? EXIT_FAILURE : EXIT_SUCCESS;
}
