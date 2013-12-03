//
// kernel/src/scheduler.c
//

#include <common.h>
#include <string.h>
#include "io.h"
#include "asm.h"
#include "lock.h"
#include "interrupt.h"
#include "scheduler.h"
#include "handle_set.h"
#include "memory/kernel.h"

static handle_set * process_handles = NULL;
static handle_set *  thread_handles = NULL;

static handle_t next_phandle = 0;
static handle_t next_thandle = 0;

lock scheduler_lock = UNLOCKED;
thread * head_thrd = NULL;
process * head_proc = NULL;

void start_scheduler(void)
{
	void timer_irq();
	reg_irq(IRQ_TIMER, timer_irq);

	outb(0x43, 0x30);
	outb(0x40, 0x02);
	outb(0x40, 0x00);
}

process * schedule(const byte * data, ulong csz, ulong bsz, ulong entry, int argc, const char ** argv)
{
	extern byte preamble_code;
	extern byte preamble_size;

	int j, k;
	ulong i, n;
	ulong size, argst, preamble;
	qword extv[3];
	process * p = kalloc(sizeof(process));

	size = csz + bsz;
	for (j = 0; j < argc; j++) {
		size += strlen(argv[j]) + 1;
	}

	size = align(size, sizeof(char *));
	size += argc * sizeof(char *);
	
	p->level = PRIVILEGE_NORMAL;
	p->timeslice = 1000;
	p->code = allocate(USPACE_BOT, align(csz + bsz, PAGE_SIZE), 1);
	p->main = USPACE_BOT;
	
	cli();
	swapflop(access(cur_proc, code, NULL), p->code, 3);
	
	for (i = USPACE_BOT; i < USPACE_BOT + csz; i++) {
		atb(i) = data[i - USPACE_BOT];
	}

	for (; i < USPACE_BOT + csz + bsz; i++) {
		atb(i) = 0;
	}
	
	preamble = i;
	for (n = 0; n < (ulong)&preamble_size; n++) {
		atb(i) = (&preamble_code)[n];
		i++;
	}
	
	argst = i;

	for (j = 0; j < argc; j++) {
		for (k = 0; argv[j][k] != 0; k++) {
			atb(i++) = argv[j][k];
		}
		atb(i++) = 0;
	}

	i = align(i, sizeof(char *));
	kprintf("Process real entry at %p\n", entry);
	extv[0] = entry;
	extv[1] = i;
	extv[2] = argc;

	for (j = 0; j < argc; j++) {
		at(i, char *) = (char *)argst;
		argst += strlen(argv[j]) + 1;
		i += sizeof(char *);
	}

	swapflop(p->code, access(cur_proc, code, NULL), 7);
//	sti();

	p->first = NULL;

	getlock(&scheduler_lock);
	if (head_thrd == NULL) {
		p->next = p;
		head_proc = p;
	} else {
		p->next = head_proc->next;
		head_proc->next = p;
	}

	insert_handle(&process_handles, next_phandle++, p);
	unlock(&scheduler_lock);

	spawn(p, preamble, countof(extv), extv);
	return p;
}

thread * spawn(process * par, ulong start, int extz, const qword * extv)
{
	int i;
	qword * top = (qword *)(LOW_HALF_TOP - PAGE_SIZE);
	thread * t = kalloc(sizeof(thread));
	
	t->state = STATE_SETUP;
	t->parent = par;
	t->stack = allocate(LOW_HALF_TOP, 2*PAGE_SIZE, 0);
	t->rsp = (qword)&top[-5 - extz];

	cli();
	swapflop(access(cur_thrd, stack, NULL), t->stack, 3);

	top[-5 - extz] = start;
	top[-4 - extz] = USER_CS | 3;
	top[-3 - extz] = getfl();
	top[-2 - extz] = (qword)(top - extz);
	top[-1 - extz] = USER_DS | 3;

	for (i = 0; i < extz; i++) {
		top[-1 - i] = extv[i];
	}

	swapflop(t->stack, access(cur_thrd, stack, NULL), 7);
//	sti();

	getlock(&scheduler_lock);
	if (head_thrd == NULL) {
		t->next_sched = t;
		head_thrd = t;
	} else {
		t->next_sched = head_thrd->next_sched;
		head_thrd->next_sched = t;
	}

	insert_handle(&thread_handles, next_thandle++, t);
	unlock(&scheduler_lock);

	t->next_inproc = par->first;
	t->next_sched = head_thrd;
	par->first = t;
	head_thrd = t;
	
	t->state = STATE_READY;

	return t;
}

process * get_process(handle_t h)
{
	return handle_lookup(process_handles, h);
}

thread * get_thread(handle_t h)
{
	return handle_lookup(thread_handles, h);
}

int get_state(handle_t h)
{
	thread * t = handle_lookup(thread_handles, h);
	if (t == NULL) return STATE_INVALID;
	return t->state;
}
