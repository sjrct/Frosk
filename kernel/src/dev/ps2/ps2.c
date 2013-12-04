//
// kernel/src/dev/ps2/ps2.c
//

#include <frosk/def.h>
#include "ps2.h"
#include "../dev.h"
#include "../../io.h"
#include "../../asm.h"
#include "../../debug.h"
#include "../../interrupt.h"
#include "../../memory/kernel.h"

#define PS2_DATA 0x60
#define PS2_STAT 0x64
#define PS2_CMND 0x64

#define WAIT_TIMEOUT 1000
#define BUF_SEG_SIZE 32

STRUCT(kb_buffer) {
	int size;
	unsigned buf[BUF_SEG_SIZE];
	kb_buffer * next, * prev;
};

STRUCT(ps2_device) {
	uint port, type;
};

static device_t * ps2_devs[2];
static kb_buffer * kb_first = NULL;
static kb_buffer * kb_last = NULL;

void ps2_kb_put(unsigned sc)
{
	kb_buffer * b;

	if (kb_first == NULL || kb_first->size == BUF_SEG_SIZE) {
		b = kalloc(sizeof(kb_buffer));
		b->next = kb_first;
		b->prev = NULL;
		b->size = 0;

		kb_first->prev = b;
		kb_first = b;

		if (kb_last == NULL) kb_last = b;
	}

	kb_first->buf[kb_first->size++] = sc;
}

unsigned ps2_kb_get(void)
{
	unsigned ret;
	kb_buffer * b;

	if (kb_last != NULL) {
		assert(kb_last->size > 0);
		ret = kb_last->buf[--kb_last->size];

		if (!kb_last->size) {
			b = kb_last;
			kb_last = kb_last->prev;
			kfree(b);

			if (kb_last == NULL) kb_first = NULL;
		}
	} else {
		ret = EOI;
	}

	return ret;
}

ulong ps2_kb_read(device_t * d, void * vbuf, ulong size)
{
	ulong i;
	unsigned sc;
	unsigned * buf = vbuf;

	for (i = 0; i < size; i++) {
		sc = ps2_kb_get();
		if (sc == EOI) break;
		buf[i] = sc;
	}

	return i;
}

static int wait_read(void)
{
	int timeout = WAIT_TIMEOUT;
	while (!(inb(PS2_STAT) & 1) && timeout) timeout--;
	return timeout;
}

static int wait_write(void)
{
	int timeout = WAIT_TIMEOUT;
	while ((inb(PS2_STAT) & 2) && timeout) timeout--;
	return timeout;
}

static void flush_output(void)
{
	while (inb(PS2_STAT) & 1) {
		inb(PS2_DATA);
	}
}

static uchar get_config(void)
{
	outb(PS2_CMND, 0x20);
	do_assert(wait_read());
	return inb(PS2_DATA);
}

static void chg_config(uchar clr, uchar set)
{
	uchar old = get_config();
	outb(PS2_CMND, 0x60);
	do_assert(wait_write());
	outb(PS2_DATA, (old & ~clr) | set);
}

static void ps2_command(uint port, uchar cmd, uchar * buf, int sz)
{
	int i;

	if (port) outb(PS2_CMND, 0xD4);

	do_assert(wait_write());
	outb(PS2_DATA, cmd);
	do_assert(wait_read());

	if (inb(PS2_DATA) != 0xFA) {
		assert(0);
	}

	if (buf != NULL) {
		for (i = 0; wait_read() && i < sz; i++) {
			buf[i] = inb(PS2_DATA);
		}
	}
}

// device should be disabled
static void detect_device(uint port, uchar enable, uchar test, uchar mask)
{
	ps2_device * ps2d;
	uchar check = 0;
	ushort type = 0;

	if (get_config() & mask) {
		outb(PS2_CMND, enable);

		if (!(get_config() & mask)) {
			outb(PS2_CMND, test);
			do_assert(wait_read());

			if (!inb(PS2_DATA)) {
				ps2_command(port, 0xFF, &check, 1);

				if (check == 0xAA) {
					ps2_command(port, 0xF5, NULL, 0);
					ps2_command(port, 0xF2, (uchar *)(&type), 2);

					if (type == PS2_NO_DEVICE) {
						type = PS2_KB_NORMAL;
					}

					if (type == PS2_KB_NORMAL || type == PS2_KB_MF2) {
						ps2_devs[port] = create_stream(
								sizeof(unsigned),
								ps2_kb_enable,
								ps2_kb_disable,
								ps2_kb_read,
								NULL
						);

						ps2d = kalloc(sizeof(ps2_device));
						ps2d->type = type;
						ps2d->port = port;
						ps2_devs[port]->data = ps2d;

						reg_dev(DEVICE_INPUT, ps2_devs[port]->hndl);
						enable_device(ps2_devs[port]);
					} else {
						ps2_devs[port] = NULL; //TODO: implement
					}

					kprintf("PS/2 Device %x detected.\n", type);
				}
			} else {
				outb(PS2_CMND, enable - 1);
			}
		}
	}
}

void detect_ps2(void)
{
	// disable ps/2 devices
	outb(PS2_CMND, 0xAD);
	outb(PS2_CMND, 0xA7);

	flush_output();

	// disable interrupts and translation
	chg_config(0x43, 0);

	// ps/2 controller self test
	outb(PS2_CMND, 0xAA);
	do_assert(wait_read());

	if (inb(PS2_DATA) != 0x55) {
		kputs("PS/2 Controller self test failed.\n");
		return;
	}

	// individual device detection
	detect_device(0, 0xAE, 0xAB, 0x10);
	detect_device(1, 0xA8, 0xA9, 0x20);
}

/*
void setup_ps2(void)
{
	void ps2_kb_int(void);
	void ps2_ms_int(void);

	// TODO determine from ACPI if PS/2 controller exists

	// disable ps/2 devices
	outb(PS2_CMND, 0xAD);
	outb(PS2_CMND, 0xA7);

	flush_output();

	// disable interrupts and translation
	chg_config(0x43, 0);

	// ps/2 controller self test
	outb(PS2_CMND, 0xAA);
	do_assert(wait_read());

	if (inb(PS2_DATA) != 0x55) {
		kputs("PS/2 Controller self test failed.\n");
		return;
	}

	// individual device detection
	detect_device(0, 0xAE, 0xAB, 0x10);
	detect_device(1, 0xA8, 0xA9, 0x20);

	// register IRQ's
	reg_irq(IRQ_PS2_KEYBOARD, ps2_kb_int);
	reg_irq(IRQ_PS2_MOUSE,    ps2_ms_int);

	// enable ps2 devices
	ps2_enable(1);
	ps2_enable(0);
}

void ps2_enable(uint port)
{
	if (ps2_devices[port] != PS2_NO_DEVICE) {
		ps2_command(port, 0xF4, NULL, 0);
		chg_config(0, 1 << port);
	} else {
		kputs("Tried to enable non-existant PS/2 device!\n");
	}
}
*/
void ps2_kb_enable(device_t * d)
{
	void ps2_kb_int(void);

	ps2_device * dev = d->data;

	if (dev->type == PS2_KB_NORMAL || dev->type == PS2_KB_MF2) {
		ps2_command(dev->port, 0xF4, NULL, 0);
		chg_config(0, 1 << dev->port);
		reg_irq(IRQ_PS2_KEYBOARD, ps2_kb_int);
	} else {
		//TODO
		assert(0);
	}
}

void ps2_kb_disable(device_t * d)
{
	// TODO
	assert(0);
}
