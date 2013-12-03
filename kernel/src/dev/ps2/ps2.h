//
// kernel/src/dev/ps2/ps2.h
//

#ifndef PS2_H
#define PS2_H

#include <util.h>
#include <types.h>
#include "../dev.h"

#define PS2_KB_NORMAL 0xAB
#define PS2_KB_MF2    0x83AB
#define PS2_MS_NORMAL 0
#define PS2_MS_SCROLL 3
#define PS2_MS_5BTN   5
#define PS2_NO_DEVICE 0xFF

void detect_ps2(void);

void ps2_kb_put(unsigned);
unsigned ps2_kb_get(void);
ulong ps2_kb_read(device_t *, void *, ulong);

void ps2_kb_enable(device_t *);
void ps2_kb_disable(device_t *);

#endif
