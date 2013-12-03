//
// prgm/frash/src/main.c
//

#include <frosk.h>
#include <matte.h>
#include "exec.h"

#define BUF_SIZE 256

void get_string(char * buf, int max)
{

}

int main(int argc, char ** argv)
{
    char inp[BUF_SIZE];

	matte_init();
	matte_puts("Frash\n");

	while (1) {
		matte_puts("> ");
		get_string(inp, BUF_SIZE);
		exec(inp);
	}

	while (1);
}

