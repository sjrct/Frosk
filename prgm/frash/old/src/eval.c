//
// eval.c
//

#include <stdio.h>
#include <frosk.h>
#include "eval.h"
#include "parse.h"

void push_all_rsrc(charls ls, FILE * f, const char * type)
{
	int i;
	if (ls.sz > 0) {
		push_rsrc(f, type);
		for (i = 0; i < ls.sz; i++) {
			set_rsrc(f, ls.ls[i], type);
		}
	}
}

int eval(const char * line)
{
	int i;
	parse_t * pr = parse(line);
	
	push_all_rsrc(pr->inf,  stdin,  "r");
	push_all_rsrc(pr->outf, stdout, "w");
	push_all_rsrc(pr->errf, stdout, "w");
	
	if (pr->args.sz > 0) {
		spawn(pr->args.sz, pr->args.ls);
	}
	
	pop_rsrc(stdin,  "r");
	pop_rsrc(stdout, "w");
	pop_rsrc(stderr, "w");
}
