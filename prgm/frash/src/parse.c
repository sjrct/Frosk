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
	char c;
	parse_st * p = malloc(sizeof(parse_st));

	p->argc = 0;

	do {
		p->argv[p->argc++] = untilws(&line);
		c = *line;
		*line = 0;
		line++;
	} while (c != 0 && p->argc < MAX_ARGC);

	return p;
}
