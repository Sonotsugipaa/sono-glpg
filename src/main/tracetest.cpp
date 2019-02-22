#include "trace.hpp"



int main(int n, char** c) {
	gla::trace_sigaction_init();  TRACE;

	return *((int*) nullptr);

	gla::trace_sigaction_restore();
}
