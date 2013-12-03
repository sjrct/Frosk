//
// include/lib/frosk/state.h
// include/prgm/frosk/state.h
// include/kernel/frosk/state.h
//

#ifndef _FROSK_STATE_H_
#define _FROSK_STATE_H_

enum _thread_state {
	STATE_INVALID = -1,
	STATE_READY = 0,
	STATE_SETUP,
	STATE_RUNNING,
	STATE_WAITING,
	STATE_STOPPED,
};

#endif
