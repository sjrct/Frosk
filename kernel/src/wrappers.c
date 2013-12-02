//
// kernel/src/wrappers.c
//

#include "io.h"

ulong do_i_live(void)
{
	kputs("Yes.\n");
	return 1;
}
