//
// kernel/src/serial.c
//

#include "serial.h"
#include "../../io.h"
#include "../../asm.h"

serial_t serials[4];

void setup_serial(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		serials[i] = ((serial_t *)0x400)[i];
	}
}

void serial_putc(char c, serial_t port)
{
	while (~inb(port + 5) & 0x20);
	outb(port, c);
}
