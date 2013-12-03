//
// prgm/frash/src/parse.c
//

#include <ctype.h>
#include <stdlib.h>
#include "parse.h"

static char * untilws(char ** s)
{
	char * ret;
	while (isspace(**s)) ++*s;
	ret = *s;
	while (!isspace(**s) && **s != 0) ++*s;
	return ret;
}


parse_st * parse(char * line)
{
	parse_st * p = malloc(sizeof(parse_st));

	p->command = untilws(&line);
	p->argc = 0;

	while (*line != 0 && p->argc < MAX_ARGC) {
		*line = 0;
		line++;
		p->argv[p->argc++] = untilws(&line);
	}

	return p;
}
