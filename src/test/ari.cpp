#include "sneka/asset/ari.hpp"
#include "sneka/asset/except.hpp"

#include "sneka/test.hpp"



const std::string ari_str[5] = {
	"mesh:/level",
	"mesh:local/level",
	"mesh:local/",
	"mesh:/",
	":/level"
};

template<unsigned i>
bool test_ari(Ari ari) {
	bool r = ari.getSerial() == ari_str[i];
	//cout << ari.getSerial() << ' ' << (r? '=' : '!') << "= " << ari_str[i] << endl;
	return r;
}



int main(int argn, char**) {
	try {
		bool r =
			   run("Ari init & serial (" + ari_str[0] + ')', test_ari<0>, Ari(ari_str[0]))
			&& run("Ari init & serial (" + ari_str[1] + ')', test_ari<1>, Ari(ari_str[1]))
			&& run("Ari init & serial (" + ari_str[2] + ')', test_ari<2>, Ari(ari_str[2]))
			&& run("Ari init & serial (" + ari_str[3] + ')', test_ari<3>, Ari(ari_str[3]))
			&& run("Ari init & serial (" + ari_str[4] + ')', test_ari<4>, Ari(ari_str[4]));
		return r? EXIT_SUCCESS : EXIT_FAILURE;
	} catch(AriException& ex) {
		std::cerr << "An ARI exception has occurred.\n" << ex.description << std::endl;
	}
}
