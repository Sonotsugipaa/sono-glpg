#include "sneka/asset/ari.hpp"
#include "sneka/asset/except.hpp"

#include "sneka/test.hpp"



bool test_construct(Ari ari) {
	cout << "ARI serial: " << ari.getSerial() << endl;
	return true;
}



int main(int argn, char**) {
	try {
		bool r =
			   run("Ari constructor", test_construct, Ari("mesh:/level"))
			&& run("Ari constructor", test_construct, Ari("mesh:local/level"))
			&& run("Ari constructor", test_construct, Ari("mesh:local/"))
			&& run("Ari constructor", test_construct, Ari("mesh:/"))
			&& run("Ari constructor", test_construct, Ari(":/level"));
		return r? EXIT_SUCCESS : EXIT_FAILURE;
	} catch(AriException& ex) {
		std::cerr << "An ARI exception has occurred.\n" << ex.description << std::endl;
	}
}
