//
// directive.h
//

#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "fs.h"

STRUCT(directive_class);
STRUCT(directive);
ENUM(derror);

struct directive_class {
	int mark;
	int (*confirm)(directive *);
	int field_count;
	char ** fields;
};

struct directive {
	directive_class * cls;
	node * nd, * par;
	char ** values;
};

directive * get_directive(int c, node *);
int confirm_directive(directive *);
int set_field(directive *, const char *, char *);

#endif
