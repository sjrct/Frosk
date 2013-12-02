//
// lib/frosk/src/textdisp.h
//

#include <frosk/textdisp.h>

#define BUFFER_SIZE 128

static int inited = 0;
static char buffer[BUFFER_SIZE];
static int bufi = 0;

static void try_init(void)
{
	if (!inited) {
		inited = 1;
	}
}

void tb_clear(void)
{
	// TODO
	bufi = 0;
}

void tb_flush(void)
{
	// TODO
	bufi = 0;
}

void tb_putc(char c)
{
	if (bufi >= BUFFER_SIZE) flush();
	buffer[bufi++] = c;
	if (c == '\n') flush();
}

void tb_puts(const char * str)
{
	for (; *str != 0; str++) putc(*str);
}
