//
// opts.c
//

#include <stdlib.h>
#include "opts.h"

opts_t get_opts(int argc, char ** argv)
{
	opts_t opts;
	filels * fls;
	int i;
	int intr_exp = 0;
	
	opts.files = NULL;
	opts.error = 0;
	opts.quiet = 0;
	opts.version = 0;
	opts.interactive = 1;
	
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'i':
				intr_exp = 1;
				opts.interactive = 1;
				break;
			
			case 'q':
				opts.quiet = 1;
				break;
				
			case 'v':
				opts.version = 1;
				break;
			
			default:
				fprintf("Unrecognized option '-%c'.\n", argv[i][1]);
				exit(-1);
			}
		} else {
			if (!intr_exp) {
				opts.interactive = 0;
			}
			
			fls = malloc(sizeof(filels));
			fls->next = opts.files;
			fls->file = argv[i];
		}
	}
	
	return opts;
}
