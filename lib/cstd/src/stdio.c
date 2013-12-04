//
// lib/cstd/src/stdio.c
//

#include <stdio.h>
#include <frosk.h>

static handle_t parent;

static void memo(void)
{
	static int nfirst = 0;
	if (!nfirst) {
		// TODO
		nfirst = 1;
	}
}

int fputc(int c, FILE * f)
{
	memo();
	_poll_write(
	return c;
}

int fputs(const char * s, FILE * f)
{
	memo();
	// TODO
	return 0;
}

int puts(const char * s)
{
	int ret;
	ret = fputs(s, stdout);
	fputc('\n', stdout);
	return ret;
}

int vfprintf(FILE * file, const char * fmt, va_list ls)
{
	// TODO
	return 0;
}

int fprintf(FILE * file, const char * fmt, ...)
{
	int ret;
	va_list ls;

	va_start(ls, fmt);
	ret = vfprintf(file, fmt, ls);
	va_end(ls);

	return ret;
}
