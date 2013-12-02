//
// parse.c
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

#define REALLOC_BY 0x10

static parse_t * new_parse_t(void)
{
	parse_t * pr = malloc(sizeof(parse_t));
	
	pr->args.sz = 0;
	pr->outf.sz = 0;
	pr->inf.sz  = 0;
	pr->errf.sz = 0;
	pr->args.ls = NULL;
	pr->outf.ls = NULL;
	pr->inf.ls  = NULL;
	pr->errf.ls = NULL;
	pr->next = NULL;
	
	return pr;
}

static char * extract(const char ** ptr)
{
	int quote;
	int i = 0;
	char * to = malloc(1);
	const char * from = *ptr;
	
	while (isspace(*from)) from++;

	quote = (*from == '"');
	if (quote) from++;
	
	while (*from != 0) {
		if (*from == '\\') {
			from++;
		} else {
			if (quote  && *from == '"') break;
			if (!quote && isspace(*from)) break;
		}
				
		to[i++] = *from++;

		if (i % REALLOC_BY == 0) {
			to = realloc(to, i + REALLOC_BY + 1);
		}
	}
	
	to[i] = 0;
	if (quote && *from == '"') from++;
	*ptr = from;
	
	return to;
}

static void parseh(const char * line, parse_t * pr, charls * ls)
{
	int err;

	while (isspace(*line)) line++;
	
	switch (*line) {
	case 0:
		return;
	
	case '<':
		line++;
		parseh(line, pr, &pr->inf);
		break;
	
	case '>':
		line++;
		err = (*line == '!');
		if (err) line++;
		
		if (err) parseh(line, pr, &pr->errf);
		else parseh(line, pr, &pr->outf);
		break;
	
	case '|':
		line++;
		pr->next = new_parse_t();
		parseh(line, pr->next, &pr->next->args);
		break;
	
	default:
		ls->ls = realloc(ls->ls, sizeof(char*) * (ls->sz + 1));
		ls->ls[ls->sz++] = extract(&line);
		parseh(line, pr, ls);
		break;
	}
	
}

parse_t * parse(const char * line)
{
	parse_t * pr = new_parse_t();
	parseh(line, pr, &pr->args);
	return pr;
}
