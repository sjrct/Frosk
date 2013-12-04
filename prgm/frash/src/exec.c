//
// prgm/frash/src/exec.c
//

#include <string.h>
#include <frosk.h>
#include <matte.h>
#include <frosk/state.h>
#include "exec.h"
#include "parse.h"

#define MAX_PATH 264

int exec(parse_st * ps)
{
	char path[MAX_PATH] = {
		'p', 'r', 'g', 'm', '/', 0
	};

	char buf;
	handle_t child, got;

	strcpy(path + 5, ps->argv[0]);
	child = _fexec(path, ps->argc, ps->argv);

	if (child == INVALID_HANDLE) {
		matte_puts("I can't seem to find a program by that name.\n");
		return -1;
	}
/*
	while (_status(child) == STATE_RUNNING) {
		while ((got = _poll_port()) != INVALID_HANDLE) {
			if (got == child) {
				_read_port(&buf, 1, got);
			} else {
				_refuse_port(got);
			}
		}
	}
*/
	return 0;
}
