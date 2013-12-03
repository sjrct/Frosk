//
// kernel/src/dev/ps2/ps2.c
//

#include "ps2.h"
#include "../dev.h"
#include "../../io.h"
#include "../../asm.h"
#include "../../debug.h"
#include "../../interrupt.h"

#define PS2_DATA 0x60
#define PS2_STAT 0x64
#define PS2_CMND 0x64

#define WAIT_TIMEOUT 1000

device_t ps2_devs[2];

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

					if (type == PS2_KB_NORMAL) {
					} else {
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
