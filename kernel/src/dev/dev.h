//
// kernel/src/dev/dev.h
//

#ifndef DEV_DEV_H
#define DEV_DEV_H

#include <util.h>
#include <types.h>
#include <frosk/def.h>
#include <frosk/devtype.h>

// reflected in kernel/src/dev/dev.asm
STRUCT(device_t) {
	device_type type;
	handle_t hndl;
	unsigned block_size;
	int enabled;
	void * data;

	void (* enable) (device_t *);
	void (* disable)(device_t *);

	union {
		struct {
			ulong (* read) (device_t *, void *, ulong);
			ulong (* write)(device_t *, void *, ulong);
		} stream;

		struct {
			ulong (* read) (device_t *, void *, ulong, ulong);
			ulong (* write)(device_t *, void *, ulong, ulong);
		} arbitrary;

		struct {
			void (* outch  )(device_t *, char, int, int);
			void (* clear  )(device_t *);

			void (* setink )(device_t *, unsigned, unsigned);
			uint (* getinkf)(device_t *);
			uint (* getinkb)(device_t *);

			void (* modcur )(device_t *, unsigned);
			void (* setcur )(device_t *, int, int);
			int  (* getcurx)(device_t *);
			int  (* getcury)(device_t *);

			int  (* width  )(device_t *);
			int  (* height )(device_t *);
		} text_disp;

		struct {
			unsigned (* getsc)(void);
		} keyboard;

//		struct {
//		} mouse;
	} api;
};

void init_devs(void);

void reg_dev(device_register, handle_t);
handle_t dev_of(device_register);
device_t * get_dev(handle_t);
device_type get_dev_type(handle_t);

void calldev(handle_t, int, ...);

void enable_device (device_t *);
void disable_device(device_t *);

device_t * create_stream(
	unsigned,
	void  (*)(device_t *),
	void  (*)(device_t *),
	ulong (*)(device_t *, void *, ulong),
	ulong (*)(device_t *, void *, ulong)
);

device_t * create_arbitrary(
	unsigned,
	void  (*)(device_t *),
	void  (*)(device_t *),
	ulong (*)(device_t *, void *, ulong, ulong),
	ulong (*)(device_t *, void *, ulong, ulong)
);

device_t * create_text_disp(
	void (*)(device_t *),
	void (*)(device_t *),

	void (*)(device_t *, char, int, int),
	void (*)(device_t *),

	void (*)(device_t *, unsigned, unsigned),
	uint (*)(device_t *),
	uint (*)(device_t *),

	void (*)(device_t *, unsigned),
	void (*)(device_t *, int, int),
	int  (*)(device_t *),
	int  (*)(device_t *),

	int  (*)(device_t *),
	int  (*)(device_t *)
);

#endif

