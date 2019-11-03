#include "sneka/asset/mesh.hpp"
#include "sneka/asset/except.hpp"

#include "sneka/test.hpp"



bool test_copy_constr(Mesh* m1) {
	Mesh m2 = *m1;
	return m2[0][2] == 1.0f;
}

bool test_move_constr(Mesh m1) {
	Mesh m2 = std::move(m1);
	return m2[0][2] == 1.0f;
}

bool test_copy_assign(Mesh* m1) {
	Mesh m2;
	m2 = *m1;
	return m2[0][2] == 1.0f;
}

bool test_move_assign(Mesh m1) {
	Mesh m2;
	m2 = std::move(m1);
	return m2[0][2] == 1.0f;
}

bool test_size(Mesh* m) {
	return m->size() == 2;
}



bool run_tests() {
	Mesh test_mesh = Mesh(Ari("mesh:/test"), 2);
	test_mesh[0][2] = 1.0f;

	cout << "REMINDER: this test should run under a memory debugging tool, "
	        "such as Valgrind." << endl;

	bool r =
		   run<Mesh*>("Mesh copy-construction", test_copy_constr, &test_mesh)
		&& run<Mesh>("Mesh move-construction", test_move_constr, test_mesh)
		&& run<Mesh*>("Mesh copy-assignment", test_copy_assign, &test_mesh)
		&& run<Mesh>("Mesh move-assignment", test_move_assign, test_mesh)
		&& run<Mesh*>("Correct size", test_size, &test_mesh);
	return r;
}



int main(int argn, char**) {
	try {
		bool r = run_tests();
		return r? EXIT_SUCCESS : EXIT_FAILURE;
	} catch(Exception& ex) {
		std::cerr << "An exception has occurred.\n" << ex.description << std::endl;
	}
}
