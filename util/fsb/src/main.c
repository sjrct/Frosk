//
// util/fsb/src/main.c
//

#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

void show_help(void)
{
	puts(
		"\nfsb (Frosk File System Builder):\n"
		"    This program interprets a file system description (.fsd) File and generates\n"
		"    a disk image for that file system for use with Frosk.\n"
		"\n"
		"usage:\n"
		"    fsb [options]\n"
		"\n"
		"options:\n"
		"    -i [file name]\n"
		"        Specifies the name of the input file (.fsd) to be used. If there are\n"
		"        multiple occurances of this option, then only the last will be used.\n"
		"        If this option is never specified, stdin will be used.\n"
		"\n"
		"    -o [file name]\n"
		"        Acts in the same manner as '-i' but specifies to the disk image file to\n"
		"        write to rather than the input file. If never specified, stdout will be\n"
		"        used.\n"
		"\n"
		"    -b [number]\n"
		"        Specifies the block size to use.\n"
		"\n"
		"    -u [string]\n"
		"        Specifies the owner of the root directory.\n"
		"\n"
		"    -p [RrWwEe]\n"
		"        Specifies the permissions for the root directory. The string can be any\n"
		"        combination of the letters r, w, and e, which indicate read, write, and\n"
		"        execute permissions respectively. A lowercase character indicates that\n"
		"        the operation is only accessible to the owner of the file; uppercase\n"
		"        indicates that the operation is accessible to everyone; no occurance of\n"
		"        a character indicates the operation is accessible to no-one.\n"
		"\n"
		"    -d\n"
		"        Output debugging information instead of generating a disk image.\n"
		"\n"
		"    -h\n"
		"        Show this helpful information.\n"
	);
}

int main(int argc, char ** argv)
{
	int i;
	source * src;
	
	FILE * in  = stdin;
	FILE * out = stdout;
	int block_size = 0x1000;
	char * root_owner = "super";
	int root_perm = FPERM_READ_OWNER | FPERM_WRITE_OWNER | FPERM_EXEC_OWNER;
	int debug = 0;
	
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'i':
				if (++i < argc) {
					if (out != stdin) fclose(in);
					in = fopen(argv[i], "r");
					input_file = argv[i];
				}
				break;

			case 'o':
				if (++i < argc) {
					if (out != stdout) fclose(out);
					out = fopen(argv[i], "w");
				}
				break;
			
			case 'b':
				if (++i < argc) block_size = atoi(argv[i]);
				break;
			
			case 'u':
				if (++i < argc) root_owner = argv[i];
				break;
			
			case 'p':
				if (++i < argc) root_perm = parse_perm(argv[i]);
				break;
			
			case 'd':
				debug = 1;
				break;
			
			case 'h':
				show_help();
				return 0;

			default:
				fprintf(stderr, "Error: Unrecognized option '%c'.\n", argv[i][1]);
				show_help();
				return -1;
			}
		} else {
			fprintf(stderr, "Error: Argument '%s' not associated with any option.\n", argv[i]);
			show_help();
			return -1;
		}
	}
	
	src = source_file(in);
	parse_fsd(src, block_size, root_perm, root_owner);
	free(src);

	if (debug) {
		debug_fs(out);
	} else {
		write_fs(out);
	}

	return 0;
}
