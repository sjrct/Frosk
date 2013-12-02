//
// lib/matte/src/init.c
//

#include <matte.h>
#include <frosk.h>
#include "matte.h"

handle_t _matte_dev;
matte_ink_t matte_color;
void (* matte_flush)(void);
void (* matte_clear)(void);

int matte_init(void)
{
	handle_t _matte_dev = _dev_of(DEVICE_DISPLAY);
	device_type t = _get_dev_type(_matte_dev);

	if (t == DEVICE_TEXT_DISP) {
		matte_text_init();
		return MATTE_TYPE_TEXT;
	}

	if (t == DEVICE_PIXEL_DISP) {
		//matte_pixel_init
		return MATTE_TYPE_PIXEL;
	}

	return MATTE_TYPE_NONE;
}
