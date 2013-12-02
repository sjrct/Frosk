//
// include/kernel/io/net/serial.h
//

#ifndef IO_NET_SERIAL_H
#define IO_NET_SERIAL_H

#include <types.h>

typedef ushort serial_t;

extern serial_t serials[4];

void setup_serial(void);
void serial_putc(char, serial_t);

#endif
