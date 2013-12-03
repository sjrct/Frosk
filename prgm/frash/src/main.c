//
// prgm/frash/src/main.c
//

#include <matte.h>
#include <frosk.h>
#include <frosk/def.h>
#include <frosk/devtype.h>
#include "exec.h"
#include "parse.h"

#define BUF_SIZE 256

static handle_t inp_dev;

// there is a better way to do this...
static const char scmap[] = {
	0, 0,
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
	0,
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	'\n',
	0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
	'`', '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'
};

void get_string(char * buf, int max)
{
	unsigned sc;
	int i = 0;

	while (1) {
		_devcall(inp_dev, DEVICE_STREAM_WRITE, &sc, 1);
		if (sc == '\n') break;

		if (sc != EOI && sc < sizeof(scmap) && scmap[sc] != 0) {
			matte_putc(scmap[sc]);
			matte_flush();
			buf[i++] = scmap[sc];
		}
	}

	buf[i] = 0;
}

int main(int argc, char ** argv)
{
	parse_st * ps;
	char inp[BUF_SIZE];

	matte_init();
	matte_puts("Frash\n");
	inp_dev = _dev_of(DEVICE_INPUT);

	if (_get_dev_type(inp_dev) != DEVICE_STREAM) {
		matte_puts("Frash error: Unknown input device style.\n");
		while (1);
	}

	while (1) {
		matte_puts("> ");
		get_string(inp, BUF_SIZE);
//		ps = parse(inp);
//		exec(ps);
	}

	while (1);
}

