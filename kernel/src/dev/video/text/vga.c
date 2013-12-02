//
// kernel/src/dev/video/text/vga.c
//

#include "vga.h"
#include "../../../io.h"
#include "../../../asm.h"
#include "../../../memory/kernel.h"

#define VGA_MEMORY 0xB8000
#define VGA_POS(X,Y,W)    ((ulong) (VGA_MEMORY + (Y)*(W)*2 + (X)*2))
#define VGA_OFFSET(X,Y,W) ((VGA_POS(X, Y, W) - VGA_MEMORY) / 2)
#define VGA_DATA(X)       (* (vga_text_dev *) (X)->data)

#define DEFAULT_WIDTH  80
#define DEFAULT_HEIGHT 25
#define DEFAULT_INK    0xf

void detect_vga(void)
{
	// FIXME: presumes the existence of a single device

	device_t * d = create_text_disp(
		vga_text_enable,
		vga_text_disable,
		vga_text_outch,
		vga_text_clear,
		vga_text_setink,
		vga_text_getinkf,
		vga_text_getinkb,
		vga_text_modcur,
		vga_text_setcur,
		vga_text_getcurx,
		vga_text_getcury,
		vga_text_width,
		vga_text_height
	);

	kprintf("d->hndl = %d\n", d->hndl);

	d->data = kalloc(sizeof(vga_text_dev));
	enable_device(d);
	reg_dev(DEVICE_DISPLAY, d->hndl);
}

void vga_text_enable(device_t * d)
{
	kprintf("enable()\n");

	VGA_DATA(d).ink = DEFAULT_INK;
	VGA_DATA(d).width  = DEFAULT_WIDTH;
	VGA_DATA(d).height = DEFAULT_HEIGHT;

	vga_text_clear(d);
	vga_text_setcur(d, 0, 0);
}

void vga_text_disable(device_t * d)
{

	kprintf("disable()\n");
	return;
}

void vga_text_outch(device_t * d, char c, int x, int y)
{
	atb(VGA_POS(x, y, VGA_DATA(d).width) + 0) = c;
	atb(VGA_POS(x, y, VGA_DATA(d).width) + 1) = VGA_DATA(d).ink;
}

void vga_text_clear(device_t * d)
{
	int x, y;
	kprintf("clear()\n");
	for (x = 0; x < VGA_DATA(d).width; x++) {
		for (y = 0; y < VGA_DATA(d).height; y++) {
			vga_text_outch(d, 0, x, y);
		}
	}
}

void vga_text_setink(device_t * d, unsigned f, unsigned b)
{
	kprintf("setink()\n");
	VGA_DATA(d).ink = ((b & 0xf) << 4) | (f & 0xf);
}

uint vga_text_getinkf(device_t * d)
{
	kprintf("getinkf()\n");
	return VGA_DATA(d).ink & 0xf;
}

uint vga_text_getinkb(device_t * d)
{
	kprintf("getinkb()\n");
	return (VGA_DATA(d).ink >> 4) & 0xf;
}

void vga_text_modcur(device_t * d, unsigned style)
{
	// TODO
	kprintf("modcur()\n");
}

void vga_text_setcur(device_t * d, int x, int y)
{
	kprintf("setcur()\n");
	ushort cur = VGA_OFFSET(x, y, VGA_DATA(d).width);

	VGA_DATA(d).curx = x;
	VGA_DATA(d).cury = y;

	outb(0x3d4, 0xf);
	outb(0x3d5, LB(cur));
	outb(0x3d4, 0xe);
	outb(0x3d5, HB(cur));
}

int vga_text_getcurx(device_t * d)
{
	kprintf("getcurx()\n");
	return VGA_DATA(d).curx;
}

int vga_text_getcury(device_t * d)
{
	kprintf("getcury()\n");
	return VGA_DATA(d).cury;
}

int vga_text_width(device_t * d)
{
	kprintf("width()\n");
	return VGA_DATA(d).width;
}

int vga_text_height(device_t * d)
{
	kprintf("height = %d\n", VGA_DATA(d).height);
	return VGA_DATA(d).height;
}

