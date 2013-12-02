//
// kernel/src/dev/video/text/vga.h
//

#ifndef DEV_VIDEO_VGA_H
#define DEV_VIDEO_VGA_H

#include <util.h>
#include <types.h>
#include "../../dev.h"

STRUCT(vga_text_dev) {
	ushort width, height;
	ushort curx, cury;
	uchar ink;
};

void detect_vga(void);

void vga_text_enable (device_t *);
void vga_text_disable(device_t *);

void vga_text_outch  (device_t *, char, int, int);
void vga_text_clear  (device_t *);

void vga_text_setink (device_t *, unsigned, unsigned);
uint vga_text_getinkf(device_t *);
uint vga_text_getinkb(device_t *);

void vga_text_modcur (device_t *, unsigned);
void vga_text_setcur (device_t *, int, int);
int  vga_text_getcurx(device_t *);
int  vga_text_getcury(device_t *);

int vga_text_width (device_t *);
int vga_text_height(device_t *);

#endif
