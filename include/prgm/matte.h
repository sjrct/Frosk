//
// include/prgm/matte.h
// include/lib/matte.h
//

#ifndef _MATTE_H_
#define _MATTE_H_

#define MATTE_TYPE_NONE  0
#define MATTE_TYPE_TEXT  1
#define MATTE_TYPE_PIXEL 2

#define MATTE_COLOR16_BLACK   0
#define MATTE_COLOR16_BLUE    1
#define MATTE_COLOR16_GREEN   2
#define MATTE_COLOR16_AQUA    3
#define MATTE_COLOR16_RED     4
#define MATTE_COLOR16_PURPLE  5
#define MATTE_COLOR16_YELLOW  6
#define MATTE_COLOR16_WHITE   7
#define MATTE_COLOR16_GRAY    8
#define MATTE_COLOR16_LBLUE   9
#define MATTE_COLOR16_LGREEN  10
#define MATTE_COLOR16_LAQUA   11
#define MATTE_COLOR16_LRED    12
#define MATTE_COLOR16_LPURPLE 13
#define MATTE_COLOR16_LYELLOW 14
#define MATTE_COLOR16_BWHITE  15

typedef unsigned matte_color_t;

typedef struct {
	matte_color_t fore, back;
} matte_ink_t;

extern matte_ink_t matte_ink;
extern void (* matte_flush)(int);
extern void (* matte_clear)(void);

int matte_init(void);

// text
void matte_text_flush(int);
void matte_text_clear(void);

void matte_outch(char, int, int);

void matte_putc(char);
void matte_puts(char *);

void matte_set_cursor(int, int);
int matte_get_cursorx(void);
int matte_get_cursory(void);

void matte_setink(matte_color_t, matte_color_t);

// pixel
// TODO

#endif
