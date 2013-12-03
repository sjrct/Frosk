//
// kernel/src/dev/dev.c
//

#include "dev.h"
#include "ps2/ps2.h"
#include "disk/ata.h"
#include "video/text/vga.h"
#include "../handle_set.h"
#include "../memory/kernel.h"

handle_set * devices = NULL;
static handle_t registers[DEVICE_MAX_REG];

void init_devs(void)
{
	int i;
	for (i = 0; i < DEVICE_MAX_REG; i++) registers[i] = INVALID_HANDLE;

	detect_ata();
//	detect_ps2();
	detect_vga();
}

void reg_dev(device_register i, handle_t hndl)
{
	if (i < DEVICE_MAX_REG) {
		registers[i] = hndl;
	}
}

handle_t dev_of(device_register i)
{
	if (i < DEVICE_MAX_REG) {
		return registers[i];
	}
	return INVALID_HANDLE;
}

device_t * get_dev(handle_t h)
{
	return handle_lookup(devices, h);
}

device_type get_dev_type(handle_t h)
{
	device_t * d = handle_lookup(devices, h);
	if (d == NULL) return DEVICE_INVALID;
	return d->type;
}

void enable_device(device_t * d)
{
	if (!d->enabled) {
		d->enabled = 1;
		if (d->enable != NULL) d->enable(d);
	}
}

void disable_device(device_t * d)
{
	if (d->enabled) {
		d->enabled = 0;
		if (d->disable != NULL) d->disable(d);
	}
}

static device_t * new_dev(device_type type, unsigned block_size)
{
	static handle_t next_hndl = 0;
	device_t * d = kalloc(sizeof(device_t));

	d->block_size = block_size;
	d->type = type;
	d->enabled = 0;
	d->data = NULL;
	d->hndl = next_hndl;
	
	insert_handle(&devices, next_hndl++, d);

	return d;
}

device_t * create_stream(
	unsigned block_size,
	void  (* enable )(device_t *),
	void  (* disable)(device_t *),
	ulong (* read   )(device_t *, void *, ulong),
	ulong (* write  )(device_t *, void *, ulong)
) {
	device_t * d = new_dev(DEVICE_STREAM, block_size);

	d->enable  = enable;
	d->disable = disable;
	d->api.stream.read  = read;
	d->api.stream.write = write;

	return d;
}

device_t * create_arbitrary(
	unsigned block_size,
	void  (* enable )(device_t *),
	void  (* disable)(device_t *),
	ulong (* read   )(device_t *, void *, ulong, ulong),
	ulong (* write  )(device_t *, void *, ulong, ulong)
) {
	device_t * d = new_dev(DEVICE_ARBITRARY, block_size);

	d->enable  = enable;
	d->disable = disable;
	d->api.arbitrary.read  = read;
	d->api.arbitrary.write = write;

	return d;
}

device_t * create_text_disp(
	void  (* enable )(device_t *),
	void  (* disable)(device_t *),

	void (* outch  )(device_t *, char, int, int),
	void (* clear  )(device_t *),

	void (* setink )(device_t *, unsigned, unsigned),
	uint (* getinkf)(device_t *),
	uint (* getinkb)(device_t *),

	void (* modcur )(device_t *, unsigned),
	void (* setcur )(device_t *, int, int),
	int  (* getcurx)(device_t *),
	int  (* getcury)(device_t *),

	int  (* width  )(device_t *),
	int  (* height )(device_t *)
) {
	device_t * d = new_dev(DEVICE_TEXT_DISP, 1);

	d->enable  = enable;
	d->disable = disable;

	d->api.text_disp.outch = outch;
	d->api.text_disp.clear = clear;

	d->api.text_disp.setink  = setink;
	d->api.text_disp.getinkf = getinkf;
	d->api.text_disp.getinkb = getinkb;

	d->api.text_disp.modcur  = modcur;
	d->api.text_disp.setcur  = setcur;
	d->api.text_disp.getcurx = getcurx;
	d->api.text_disp.getcury = getcury;

	d->api.text_disp.width  = width;
	d->api.text_disp.height = height;

	return d;
}
