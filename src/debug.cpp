#include "debug.hpp"

#include <iostream>



void debug(std::string str) {
	std::cout << str;
}

void debugln(std::string str) {
	std::cout << str << std::endl;
}

void debugln() {
	std::cout << std::endl;
}
