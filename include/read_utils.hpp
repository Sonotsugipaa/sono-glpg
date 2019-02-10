#include <cstdio>



// returns -1 on error
int ch_to_digit(char character);

// prints uppercase letters
char digit_to_ch(unsigned int number);

bool read_int(const char * string, int* destination, int radix);

bool read_dec_int(const char * string, int* destination);

bool fprint_int(FILE* file, int number, int base);
