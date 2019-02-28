#include "trace.hpp"



int main() {
	gla::trace_sigaction_init();  TRACE;

	return *((int*) nullptr);

	gla::trace_sigaction_restore();
}
