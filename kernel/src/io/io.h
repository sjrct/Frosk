//
// kernel/src/io/io.h
//

#ifndef IO_IO_H
#define IO_IO_H

#include <types.h>
#include "../dev/dev.h"
/*
void setup_io(void);

extern int rwb_size;
extern ulong (*bread) (byte *, ulong, ulong);
extern ulong (*bwrite)(byte *, ulong, ulong);
*/
ulong read (device_t *, byte *, ulong, ulong);
ulong write(device_t *, byte *, ulong, ulong);

#endif
