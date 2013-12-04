//
// kernel/src/wrappers.c
//

#include <frosk/def.h>
#include "io.h"
#include "exec.h"
#include "scheduler.h"

ulong do_i_live(void)
{
	kputs("Yes.\n");
	return 1;
}

handle_t fexec_sc(const char * prgm, int argc, const char ** argv)
{
	process * p = fexec(prgm, argc, argv);
	if (p == NULL) return INVALID_HANDLE;
	return p->first->handle;
}
