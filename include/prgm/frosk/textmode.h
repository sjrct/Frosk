//
// include/prgm/frosk/textdisp.h
// include/lib/frosk/textdisp.h
//

#ifndef _FROSK_TEXTDISP_H_
#define _FROSK_TEXTDISP_H_

#include <types.h>

void outch(char, int, int);
void clear(void);
void setink(uint, uint);
uint getinkf(void);
uint getinkb(void);
void setcur(int, int);
int getcurx(void);
int getcury(void);

#endif
