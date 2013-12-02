//
// frash.c
//

#include <stdio.h>
#include <stdlib.h>
#include "frash.h"
#include "opts.h"
#include "eval.h"

#define REALLOC_BY 0x40

char * get_line(FILE * f)
{
	int c;
	int i = 0;
	char * str = NULL;
	
	for (;;) {
		c = fgetc(f);

		if (i % REALLOC_BY == 0) {
			str = realloc(str, i + REALLOC_BY);
		}
		
		if (c == '\n' || c == 0 || c == EOF) break;
		str[i++] = c;
	}
	
	if (i == 0 && c == EOF) {
		free(str);
		return NULL;
	}
	
	str[i] = 0;
	return str;
}

void parse_file(FILE * f)
{
	char * line;

	for (;;) {
		line = get_line(f);
		eval(line);
	}
}

void do_file(const char * file)
{
	FILE * f = fopen(file, "r");

	if (f == NULL) {
		fprintf(stderr, "Error: Cannot open file '%s'\n", file);
		return;
	}

	parse_file(f);

	fclose(f);
}

int main(int argc, char ** argv)
{
	filels * fls;
	opts_t opts = get_opts(argc, argv);
	
	if (opts.version) {
		for (fls = opts.files; fls != NULL; fls = fls->next) {
			do_file(fls->file);
		}
	
		if (opts.interactive) {
			parse_file(stdin);
		}
	} else {
		fputs("frash version FRASH_VERSION.\n");
	}
		
	return 0;
}
