//
// parse.h
//

#ifndef PARSE_H
#define PARSE_H

#include <util.h>
#include <stdio.h>
#include "fs.h"

STRUCT(source);

struct source {
	FILE * f;
	int ungot;
	int offset;
	char * str;
};

extern char * input_file;
extern int line_num;

node * parse_directory(source *, node *);
void parse_fsd(source *, uint, uint, char *);
uint parse_perm(const char *);

source * source_file(FILE *);
source * source_string(char *);

#endif
