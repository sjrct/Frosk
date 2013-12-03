//
// prgm/frash/src/exec.c
//

#include <frosk.h>
#include <frosk/state.h>
#include "exec.h"
#include "parse.h"

int exec(parse_st * ps)
{
	char buf;
	handle_t child, got;

	child = _fexec(ps->command, ps->argc, ps->argv);

	while (_status(child) == STATE_RUNNING) {
		while ((got = _poll_port()) != INVALID_HANDLE) {
			if (got == child) {
				_read_port(&buf, 1, got);
			} else {
				_refuse_port(got);
			}
		}
	}

	return 0;
}
