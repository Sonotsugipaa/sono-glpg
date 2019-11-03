#include "sneka/asset/ari.hpp"
#include "sneka/asset/except.hpp"

#include "sneka/test.hpp"



const std::string ari_str[5] = {
	"mesh:/lvl1",
	"mesh:local/lvl1",
	"mesh:local/",
	"mesh:/",
	":/lvl1"
};
const std::string ari_null = "level:/";
const std::string ari_valid = ":/local";
const std::string ari_sub = "level:/lvl1";
const std::string ari_root = "level:local/";

template<unsigned i>
bool test_ari(Ari ari) {
	bool r = ari.getSerial() == ari_str[i];
	cout << ari.getSerial() << ' ' << (r? '=' : '!') << "= " << ari_str[i] << endl;
	return r;
}

template<bool expect>
bool test_null(Ari ari) {
	bool is_null = ari.isNull();
	cout << ari.getSerial() << " is " << (is_null? "null" : "valid")
	     << "; location: " << (ari.location.empty()? "null" : "given")
		  << "; path: " << ari.path.size() << "-long" << endl;
	return is_null == expect;
}

template<bool expect>
bool test_root(Ari ari) {
	bool is_root = ari.isRoot();
	return is_root == expect;
}



int main(int argn, char**) {
	try {
		bool r =
			   run("Ari init & serial (" + ari_str[0] + ')', test_ari<0>, Ari(ari_str[0]))
			&& run("Ari init & serial (" + ari_str[1] + ')', test_ari<1>, Ari(ari_str[1]))
			&& run("Ari init & serial (" + ari_str[2] + ')', test_ari<2>, Ari(ari_str[2]))
			&& run("Ari init & serial (" + ari_str[3] + ')', test_ari<3>, Ari(ari_str[3]))
			&& run("Ari init & serial (" + ari_str[4] + ')', test_ari<4>, Ari(ari_str[4]))
			&& run("Null ari ("+ari_null+')', test_null<true>, Ari(ari_null))
			&& run("Valid ari ("+ari_valid+')', test_null<false>, Ari(ari_valid))
			&& run("Root ari ("+ari_root+')', test_root<true>, Ari(ari_root))
			&& run("Sub-root ari ("+ari_sub+')', test_root<false>, Ari(ari_sub));
		return r? EXIT_SUCCESS : EXIT_FAILURE;
	} catch(AriException& ex) {
		std::cerr << "An ARI exception has occurred.\n" << ex.description << std::endl;
	}
}
