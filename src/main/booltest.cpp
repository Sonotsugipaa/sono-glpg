#include <iostream>
#include <limits>

#include "boolarray.hpp"

using gla::BoolArray;
using gla::BoolArrayOverflow;
using gla::mask_set;
using gla::mask_clr;



namespace {

	template<typename T>
	void print_as_bits(std::ostream& os, T n) {
		using t_limits = std::numeric_limits<T>;

		for(int i = t_limits::digits - 1; i >= 0; i-=1) {
			os << ((n >> i) & 1);
		}
	}

}


int main(int argc, char** argv) {
	(void) argc;
	(void) argv;

	std::cout << "mask_set 3, 5 ";
	print_as_bits(std::cout, mask_set(3) | mask_set(5));
	std::cout << std::endl;

	std::cout << "mask_clr 3, 5 ";
	print_as_bits(std::cout, mask_clr(3) & mask_clr(5));
	std::cout << std::endl;

	BoolArray array = BoolArray(3);

	std::cout
			<< "array = "
			<< array[0] << ", "
			<< array[1] << ", "
			<< array[2] << std::endl;
	array[1] = true;
	std::cout
			<< "array = "
			<< array[0] << ", "
			<< array[1] << ", "
			<< array[2] << std::endl;
	array.set(1, false);
	std::cout
			<< "array = "
			<< array[0] << ", "
			<< array[1] << ", "
			<< array[2] << std::endl;
	array[1] = true;

	BoolArray acopy = BoolArray(array);
	std::cout
			<< "array copy = "
			<< acopy.get(0) << ", "
			<< acopy.get(1) << ", "
			<< acopy.get(2) << std::endl;

	acopy = BoolArray::random(5, 5);
	std::cout
			<< "rnd array = "
			<< acopy[0] << ", "
			<< acopy[1] << ", "
			<< acopy[2] << ", "
			<< acopy[3] << ", "
			<< acopy[4] << std::endl;

	try {
		array[array.getSize()];
	} catch(BoolArrayOverflow& ex) {
		std::cout
				<< "Intentionally triggered a BoolArrayOverflow"
				   " with message.\n\twhat():  \""
				<< ex.what() << '"' << std::endl;
	}

	try {
		array.set(array.getSize(), true);
	} catch(BoolArrayOverflow& ex) {
		std::cout
				<< "Intentionally triggered a BoolArrayOverflow"
					" with message.\n\twhat():  \""
				<< ex.what() << '"' << std::endl;
	}

	return EXIT_SUCCESS;
}
