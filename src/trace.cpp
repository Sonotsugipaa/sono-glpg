#include "trace.hpp"

#include <iostream>



const char * gla_trace_file = "<unknown>";
unsigned int gla_trace_line = 0;



#ifdef _POSIX_C_SOURCE

	extern "C" {
		#include <string.h>

		#include <sys/types.h>
		#include <signal.h>
		#include <unistd.h>
	}



	namespace {
		struct sigaction old_sa;
		struct sigaction new_sa;
		bool init = false;

		void sighandler(int s) {
			if(s == SIGSEGV) {
				std::cerr
				<< "Segmentation fault after line " << gla_trace_line
				<< " in file \"" << gla_trace_file << "\"." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}


	namespace gla {

		void trace_sigaction_init() {
			if(init) return;

			new_sa.sa_handler = sighandler;
			sigemptyset(&new_sa.sa_mask);
			new_sa.sa_flags = 0;
			sigaction(SIGSEGV, &new_sa, &old_sa);

			init = true;
		}

		void trace_sigaction_restore() {
			if(! init) return;

			sigaction(SIGSEGV, &old_sa, NULL);

			init = false;
		}

	}

#else // #ifdef _POSIX_C_SOURCE



	namespace gla {
		void trace_sigaction_init() { }
		void trace_sigaction_restore() { }
	}

#endif // #ifdef _POSIX_C_SOURCE
