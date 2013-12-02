//
// opts.h
//

#ifndef OPTS_H
#define OPTS_H

typedef struct filels {
	char * file;
	struct filels * next;
} filels;

typedef struct {
	int interactive;
	int version;
	int quiet;
	filels * input;
} opts_t;

opts_t get_opts(int, char **);

#endif
