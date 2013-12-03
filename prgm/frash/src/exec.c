//
// prgm/frash/src/exec.c
//

#include <frosk.h>
#include <frosk/status.h>
#include "exec.h"

int exec(parse_st * ps)
{
	char buf;
    handle_t child, got;

	child = _fexec(ps->cmd, ps->argc, ps->argv);

	while (_status(child) == STATUS_RUNNING) {
		while ((got = poll_port(0)) != INVALID_HANDLE) {
			if (got == child) {
				read_port(&buf, 1, got);
			} else {
				refuse_port(got);
			}
		}
	}
}
