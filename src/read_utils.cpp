#include <cstdlib>
#include <cstring>
#include "read_utils.hpp"

// used only for float conversion
#include <string>
#include <stdexcept>



int ch_to_digit(char ch) {
	if(ch >= '0' && ch <= '9')
		return ch - '0';

	if(ch >= 'a' && ch <= 'z')
		return ch+10 - 'a';

	if(ch >= 'A' && ch <= 'Z')
		return ch+10 - 'A';

	return -1;
}

char digit_to_ch(unsigned int n) {
	if(n <= 9)
		return n + '0';

	if(n <= (10 + 'Z' - 'A'))
		return n + 'A' - 10;

	return -1;
}

bool read_int(const char * str, int* dest, int radix) {
	int acc = 0;
	char digit;
	bool neg = false;

	if(*str == '-') {
		neg = true;
		str += 1;
	}

	while((digit = *(str++)) != '\0') {
		digit = ch_to_digit(digit);
		if(digit == -1 || digit >= radix) {
			*dest = 0;
			return false;
		}

		acc = (acc*radix) + digit;

		/* only an overflow could cause the accumulator
		 * to be negative */
		if(acc < 0) return false;
	}

	*dest = acc * (neg? -1 : 1);
	return true;
}

bool read_dec_int(const char * str, int* dest) {
	return read_int(str, dest, 10);
}

/* This would like some optimization, then again
 * using std::stof(...) right away might be a better
 * choice. */
bool read_dec_float(const char * str, float* dest) {
	try {
		*dest = std::stof(str);
		return true;
	} catch(std::logic_error& err) {
		*dest = 0.0f;
		return false;
	}
}

#define MAX_DIGITS_OF(NUMBER_TYPE) (8 * sizeof NUMBER_TYPE)
bool fprint_int(FILE* file, int number, int base) {
	int write_index=0;
	int written;
	char* buffer = static_cast<char*>(malloc(MAX_DIGITS_OF(number)));
	 buffer += MAX_DIGITS_OF(number) - 1;

	if(base < 2 || base > (11 + 'z' - 'a')) return false;

	if(number < 0) {
		number = -number;
	}

	while(number > 0) {
		buffer[write_index] = digit_to_ch(number % base);
		write_index -= 1;
		number /= base;
	}

	written = fwrite(buffer + write_index, 1, 1-write_index, file);

	free(buffer + 1 - MAX_DIGITS_OF(number));
	return written == 1-write_index;
}
