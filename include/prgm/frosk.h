//
// include/prgm/frosk.h
// include/lib/frosk.h
//

#ifndef _FROSK_H_
#define _FROSK_H_

#include <types.h>
#include <frosk/def.h>
#include <frosk/devtype.h>

ulong __syscall0(ulong);
ulong __syscall1(ulong, ulong);
ulong __syscall2(ulong, ulong, ulong);
ulong __syscall3(ulong, ulong, ulong, ulong);

#ifndef INLINE
	#define INLINE __attribute__((always_inline)) static inline
#endif

enum {
	__DO_I_LIVE = 0,
	__FEXEC,
	__DEV_OF,
	__GET_DEV_TYPE,
	__DEVCALL, // reflected in lib/frosk/src/syscall.asm
};

INLINE int _do_i_live(void) {
	return __syscall0(__DO_I_LIVE);
}

INLINE handle_t _fexec(char * fn, int argc, char ** argv) {
	return __syscall3((ulong)fn, argc, (ulong)argv, __FEXEC);
}

INLINE handle_t _dev_of(device_register r) {
	return __syscall1(r, __DEV_OF);
}

INLINE device_type _get_dev_type(handle_t h) {
	return __syscall1(h, __GET_DEV_TYPE);
}

ulong _devcall(handle_t device, int index,...);

#endif
