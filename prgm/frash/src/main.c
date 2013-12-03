//
// prgm/frash/src/main.c
//

#include <frosk.h>
#include <matte.h>
#include "exec.h"
#include "parse.h"

#define BUF_SIZE 256

void get_string(char * buf, int max)
{
	buf[0] = 0;
	return;
}

int main(int argc, char ** argv)
{
	parse_st * ps;
	char inp[BUF_SIZE];

	matte_init();
	matte_puts("Frash\n");

//	while (1) {
//		matte_puts("> ");
//		get_string(inp, BUF_SIZE);
//		ps = parse(inp);
//		exec(ps);
//	}

	while (1);
}

