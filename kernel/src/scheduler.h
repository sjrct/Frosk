//
// include/kernel/scheduler.h
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <util.h>
#include <frosk/state.h>
#include "memory/region.h"

STRUCT(process);
STRUCT(thread);
ENUM(privilege);

typedef enum _thread_state thread_state;

enum privilege {
	PRIVILEGE_NORMAL = 0,
	PRIVILEGE_ELEVATED,
	PRIVILEGE_SUPER,
};

#pragma pack(push, 1)
// reflected in kernel/src/scheduler.asm
STRUCT(process) {
	process * next;
	thread * first;
	region * code;
	dword timeslice;
	uint level;
	qword main;
	char ** argv;
	int argc;
	handle_t handle;
};

STRUCT(thread) {
	process * parent;
	thread * next_inproc;
	thread * next_sched;
	region * stack;
	qword rsp;
	thread_state state;
	dword pack;
	handle_t handle;
};
#pragma pack(pop)

void start_scheduler(void);
process * schedule(const byte *, ulong, ulong, ulong, int, const char **);
thread * spawn(process *, ulong, int, const qword *);

process * get_process(handle_t);
thread * get_thread(handle_t);

int get_state(handle_t h);

extern process * head_proc;
extern thread * head_thrd;
extern thread * cur_thrd;

#define cur_proc (access(cur_thrd, parent, NULL))

#endif
