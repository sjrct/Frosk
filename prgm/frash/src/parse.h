//
// prgm/frash/src/parse.h
//

#ifndef PARSE_H
#define PARSE_H

#define MAX_ARGC 20

typedef struct {
	int argc;
	char * argv[MAX_ARGC];
} parse_st;

parse_st * parse(char * line);

#endif
