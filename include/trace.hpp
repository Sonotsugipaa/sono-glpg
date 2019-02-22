#ifndef SONOGLPG_TRACE_HPP
#define SONOGLPG_TRACE_HPP

#define TRACE {gla_trace_line=__LINE__;gla_trace_file=__FILE__;}



extern const char * gla_trace_file;
extern unsigned int gla_trace_line;

namespace gla {

	void trace_sigaction_init();
	void trace_sigaction_restore();

}

#endif
