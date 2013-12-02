//
// parse.h
//

#ifndef PARSE_H
#define PARSE_H

typedef struct {
	char ** ls;
	int sz;
} charls;

typedef struct parse_t {
	int argc;
	charls args;
	charls outf;
	charls inf;
	charls errf;
	struct parse_t * next;
} parse_t;

parse_t * parse(const char *);

#endif
