//
// kernel/src/dev/ps2/ps2.h
//

#ifndef PS2_H
#define PS2_H

#include <util.h>
#include <types.h>

#define PS2_KB_NORMAL 0xAB
#define PS2_KB_MF2    0x83AB
#define PS2_MS_NORMAL 0
#define PS2_MS_SCROLL 3
#define PS2_MS_5BTN   5
#define PS2_NO_DEVICE 0xFF

#define PS2_IS_KB(X) ((ps2_devices[X] & 0xFF) == PS2_KB_NORMAL)
#define PS2_IS_MS(X) ((ps2_devices[X] & 0xFF) != PS2_KB_NORMAL)

STRUCT(ps2_device) {
	ushort id;
	ushort type;
};

void detect_ps2(void);

#endif
