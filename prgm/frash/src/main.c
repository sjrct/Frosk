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

static void get_string(char * buf, int max)
{
	// TODO fix so not awful, not just qwerty, can be out of function, and not here in the first place
	static const char scmap[][0x70] =
	{
		{
			// scancodes from 0x00
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, '\t',  '`', 0x00,

			// scancodes from 0x10
			0x00, 0x00, 0x00, 0x00,
			0x00,  'q',  '1', 0x00,
			0x00, 0x00,  'z',  's',
			 'a',  'w',  '2', 0x00,

			// scancodes from 0x20
			0x00,  'c',  'x',  'd',
			 'e',  '4',  '3', 0x00,
			0x00,  ' ',  'v',  'f',
			 't',  'r',  '5', 0x00,

			// scancodes from 0x30
			0x00,  'n',  'b',  'h',
			 'g',  'y',  '6', 0x00,
			0x00, 0x00,  'm',  'j',
			 'u',  '7',  '8', 0x00,

			// scancodes from 0x40
			0x00,  ',',  'k',  'i',
			 'o',  '0',  '9', 0x00,
			0x00,  '.',  '/',  'l',
			 ';',  'p',  '-', 0x00,

			// scancodes from 0x50
			0x00, 0x00, '\'', 0x00,
			 '[',  '=', 0x00, 0x00,
			0x00, 0x00, '\n',  ']',
			0x00, '\\', 0x00, 0x00,

			// scancodes from 0x60
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x08, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
		},

		{
			// scancodes from 0x00
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, '\t',  '~', 0x00,

			// scancodes from 0x10
			0x00, 0x00, 0x00, 0x00,
			0x00,  'Q',  '!', 0x00,
			0x00, 0x00,  'Z',  'S',
			 'A',  'W',  '@', 0x00,

			// scancodes from 0x20
			0x00,  'C',  'X',  'D',
			 'E',  '$',  '#', 0x00,
			0x00,  ' ',  'V',  'F',
			 'T',  'R',  '%', 0x00,

			// scancodes from 0x30
			0x00,  'N',  'B',  'H',
			 'G',  'Y',  '^', 0x00,
			0x00, 0x00,  'M',  'J',
			 'U',  '&',  '*', 0x00,

			// scancodes from 0x40
			0x00,  '<',  'K',  'I',
			 'O',  ')',  '(', 0x00,
			0x00,  '>',  '?',  'L',
			 ':',  'P',  '_', 0x00,

			// scancodes from 0x50
			0x00, 0x00,  '"', 0x00,
			 '{',  '+', 0x00, 0x00,
			0x00, 0x00, '\n',  '}',
			0x00,  '|', 0x00, 0x00,

			// scancodes from 0x60
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x08, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
		},
	};

	unsigned sc = EOI;
	int i = 0;
	int lshift = 0;
	int rshift = 0;
	int cnt, c;

	while (1) {
		sc = 0;
		cnt = _devcall(inp_dev, DEVICE_STREAM_READ, &sc, 4);

		if (cnt > 0 && sc < 0x100) {
			if (sc == 0x5A) break; // 0x58 = return key

			switch (sc) {
			case 0x12:   lshift = 1; break;
			case 0xF012: lshift = 0; break;
			case 0x59:   rshift = 1; break;
			case 0xF059: rshift = 0; break;
			}

			if (sc != EOI && sc < sizeof(scmap[0]) && scmap[0][sc] != 0) {
				c = scmap[lshift || rshift][sc];

				if (c == 8) {
					if (i > 0) --i;
					else continue;
				} else {
					buf[i++] = c;
				}

				matte_putc(c);
				matte_flush(0);
			}
		}
	}

	matte_putc('\n');
	buf[i] = 0;
}

int main(int argc, char ** argv)
{
	parse_st * ps;
	char inp[BUF_SIZE];

	matte_init();
	matte_setink(MATTE_COLOR16_BWHITE, MATTE_COLOR16_BLACK);
	matte_puts("Frash\n");

	inp_dev = _dev_of(DEVICE_INPUT);
	if (_get_dev_type(inp_dev) != DEVICE_STREAM) {
		matte_puts("Frash error: Unknown input device style.\n");
		while (1);
	}

	while (1) {
		matte_puts("> ");
		matte_flush(0);

		get_string(inp, BUF_SIZE);
		ps = parse(inp);
		exec(ps);
	}

	while (1);
}

