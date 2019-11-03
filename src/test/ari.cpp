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

template<class ari_t, unsigned i>
bool test_ari(ari_t ari) {
	bool r = ari.getSerial() == ari_str[i];
	//cout << ari.getSerial() << ' ' << (r? '=' : '!') << "= " << ari_str[i] << endl;
	return r;
}

template<class ari_t, bool expect>
bool test_null(ari_t ari) {
	bool is_null = ari.isNull();
	/*cout << ari.getSerial() << " is " << (is_null? "null" : "valid")
	     << "; location: " << (ari.location.empty()? "null" : "given")
	     << "; path: " << ari.path.size() << "-long" << endl;*/
	return is_null == expect;
}

template<class ari_t, bool expect>
bool test_root(ari_t ari) {
	bool is_root = ari.isRoot();
	return is_root == expect;
}



template<class ari_t>
bool run_tests() {
	bool r =
		   run("Ari init & serial (" + ari_str[0] + ')', test_ari<ari_t, 0>, ari_t(ari_str[0]))
		&& run("Ari init & serial (" + ari_str[1] + ')', test_ari<ari_t, 1>, ari_t(ari_str[1]))
		&& run("Ari init & serial (" + ari_str[2] + ')', test_ari<ari_t, 2>, ari_t(ari_str[2]))
		&& run("Ari init & serial (" + ari_str[3] + ')', test_ari<ari_t, 3>, ari_t(ari_str[3]))
		&& run("Ari init & serial (" + ari_str[4] + ')', test_ari<ari_t, 4>, ari_t(ari_str[4]))
		&& run("Null ari ("+ari_null+')', test_null<ari_t, true>, ari_t(ari_null))
		&& run("Valid ari ("+ari_valid+')', test_null<ari_t, false>, ari_t(ari_valid))
		&& run("Root ari ("+ari_root+')', test_root<ari_t, true>, ari_t(ari_root))
		&& run("Sub-root ari ("+ari_sub+')', test_root<ari_t, false>, ari_t(ari_sub));
	return r;
}



int main(int argn, char**) {
	try {
		cout << "\n\t------- CACHED ARI -------" << endl;
		bool r = run_tests<Ari::Cached>();
		cout << "\n\t------- UNCACHED ARI -------" << endl;
		r = r && run_tests<Ari>();
		cout << '\n';
		return r? EXIT_SUCCESS : EXIT_FAILURE;
	} catch(AriException& ex) {
		std::cerr << "An ARI exception has occurred.\n" << ex.description << std::endl;
	}
}
