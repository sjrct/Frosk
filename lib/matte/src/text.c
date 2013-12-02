//
// lib/matte/src/text.c
//

#include <util.h>
#include <matte.h>
#include <frosk.h>
#include <stdlib.h>
#include "matte.h"

#define DEF_BUF_LINES 128
#define ASSERT
//#define ASSERT assert(_matte_dev == DEVICE_TEXT_DISP)

STRUCT(line) {
	int wrote, offset;
	int size;
	char * data;
	line * down, * up;
};

STRUCT(screen) {
	int base_num;
	int lines, max_lines;
	int width, height;
	int cur_col;
	line * cur_line;
	line * bottom, * top;
};

static screen scr = {
	0,
	0, DEF_BUF_LINES,
	0, 0,
	0,
	NULL,
	NULL, NULL
};

static void push_line(void)
{
	line * l;

	if (scr.lines >= scr.max_lines) {
		l = scr.top;
		l->down->up = NULL;
		l->offset = 0;
	} else {
		scr.lines++;
		l = malloc(sizeof(line));
		l->data = malloc(scr.width);
		scr.bottom->down = l;
	}

	l->size = 0;
	l->wrote = -1;
	l->up = scr.bottom;
	l->down = NULL;
	scr.bottom = l;

	if (scr.top == NULL) {
		scr.top = l;
		l->offset = 0;
	} else {
		l->offset = scr.bottom->offset + 1;
	}
}

void matte_text_init(void)
{
	ASSERT;

	matte_clear = matte_text_clear;
	matte_flush = matte_text_flush;

	_devcall(_matte_dev, DEVICE_TEXT_DISP_SETCUR, 0, 0);
	scr.width  = _devcall(_matte_dev, DEVICE_TEXT_DISP_WIDTH);
	scr.height = _devcall(_matte_dev, DEVICE_TEXT_DISP_HEIGHT);

	push_line();
	scr.cur_line = scr.bottom;
	scr.cur_col = 0;
	scr.base_num = 0;
}

void matte_text_clear(void)
{
	ASSERT;
	_devcall(_matte_dev, DEVICE_TEXT_DISP_CLEAR);
}

void matte_text_reset(void)
{
	line * l, * t;

	matte_text_clear();

	for (l = scr.bottom->up; l != NULL; l = t) {
		t = l->up;
		free(l->data);
		free(l);
	}

	scr.base_num = 0;
	scr.bottom->up = NULL;
	scr.bottom->size = 0;
	scr.bottom->wrote = 0;
	scr.top = scr.bottom;

	matte_text_clear();
}

void matte_text_flush(void)
{
	int x, y;
	line * l;

	for (l = scr.bottom; l != NULL; l = l->up) {
		y = scr.base_num + l->offset;

		if (y != l->wrote) {
			if (y >= 0 && y < scr.height) {
				for (x = 0; x < l->size; x++) {
					matte_outch(l->data[x], x, y);
				}

				for (; x < scr.width; x++) {
					matte_outch(0, x, y);
				}
			}

			l->wrote = y;
		}
	}
}

void matte_outch(char c, int x, int y)
{
	ASSERT;
	_devcall(_matte_dev, DEVICE_TEXT_DISP_OUTCH, c, x, y);
}

void matte_putc(char c)
{
	switch (c) {
	case 0x8:
		if (scr.cur_col == 0) {
			if (scr.cur_line->up != NULL) {
				scr.cur_line = scr.cur_line->up;
				scr.cur_col = scr.cur_line->size;
			}
		} else {
			scr.cur_col--;
			scr.cur_line->data[scr.cur_col] = 0;
		}
		break;

	case 0xA:
		for (; scr.cur_col < scr.width; scr.cur_col++) {
			scr.cur_line->data[scr.cur_col] = 0;
		}

		scr.cur_col = 0;
		if (scr.cur_line->down == NULL) {
			scr.cur_line = scr.cur_line->down;
		} else {
			push_line();
			scr.cur_line = scr.bottom;
		}

		matte_flush();
		break;

	default:
		scr.cur_line->data[0] = NULL;
		if (scr.cur_col > scr.cur_line->size) {
			scr.cur_line->size = scr.cur_col;
		}
		break;
	}
}

void matte_puts(char * s)
{
	while (*s != 0) {
		matte_putc(*s);
		s++;
	}
}
