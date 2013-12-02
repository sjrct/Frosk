//
// include/prgm/matte.h
// include/lib/matte.h
//

#ifndef _MATTE_H_
#define _MATTE_H_

#define MATTE_TYPE_NONE  0
#define MATTE_TYPE_TEXT  1
#define MATTE_TYPE_PIXEL 2

typedef unsigned matte_color_t;

typedef struct {
	matte_color_t fore, back;
} matte_ink_t;

extern matte_ink_t matte_color;
extern void (* matte_flush)(void);
extern void (* matte_clear)(void);

int matte_init(void);

// text
void matte_text_flush(void);
void matte_text_clear(void);

void matte_outch(char, int, int);

void matte_putc(char);
void matte_puts(char *);

void matte_set_cursor(int, int);
int matte_get_cursorx(void);
int matte_get_cursory(void);

// pixel
// TODO

#endif
