//
// kernel/src/ps2.h
//

#ifndef PS2_H
#define PS2_H

#include <types.h>

typedef dword keycode;

#define PS2_KB_NORMAL 0xAB
#define PS2_KB_MF2    0x83AB
#define PS2_MS_NORMAL 0
#define PS2_MS_SCROLL 3
#define PS2_MS_5BTN   5
#define PS2_NO_DEVICE 0xFF

#define PS2_IS_KB(X) ((ps2_devices[X] & 0xFF) == PS2_KB_NORMAL)
#define PS2_IS_MS(X) ((ps2_devices[X] & 0xFF) != PS2_KB_NORMAL)

void setup_ps2(void);
void ps2_enable(uint);

extern uint ps2_devices[];

#endif
