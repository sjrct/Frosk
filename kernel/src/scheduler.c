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
#include "memory/pages.h"
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
	ulong size, argst;
	ulong tvirt,preamble;
	qword extv[3];
	process * p = kalloc(sizeof(process));

	size = csz + bsz;
	for (j = 0; j < argc; j++) {
		size += strlen(argv[j]) + 1;
	}

	size  = align(size, sizeof(char *));
	size += argc * sizeof(char *);
	size  = align(size, PAGE_SIZE) + 2*PAGE_SIZE; // TODO actually calc argv size

	tvirt = alloc_pgs(size, VIRT_PAGES);
	p->level     = PRIVILEGE_NORMAL;
	p->timeslice = 1000;
	p->code      = allocate(tvirt, size, 1);

	swapin(p->code, 3);

	for (i = tvirt; i < tvirt + csz; i++) {
		atb(i) = data[i - tvirt];
	}

	for (; i < tvirt + csz + bsz; i++) {
		atb(i) = 0;
	}

	preamble = (i - tvirt) + USPACE_BOT;
	for (n = 0; n < (ulong)&preamble_size; n++) {
		atb(i) = (&preamble_code)[n];
		i++;
	}

	argst = (i - tvirt) + USPACE_BOT;
	for (j = 0; j < argc; j++) {
		for (k = 0; argv[j][k] != 0; k++) {
			atb(i++) = argv[j][k];
		}
		atb(i++) = 0;
	}

	i = align(i, sizeof(char *));
	extv[0] = entry;
	extv[1] = (i - tvirt) + USPACE_BOT;
	extv[2] = argc;

	for (j = 0; j < argc; j++) {
		at(i, char *) = (char *)argst;
		argst += strlen(argv[j]) + 1;
		i += sizeof(char *);
	}

	swapout(p->code);
	shift_rgn(p->code, USPACE_BOT);
	free_pgs(tvirt, size, VIRT_PAGES);

	p->first = NULL;

	getlock(&scheduler_lock);
	if (head_thrd == NULL) {
		p->next = p;
		head_proc = p;
	} else {
		p->next = head_proc->next;
		head_proc->next = p;
	}

	p->handle = next_phandle++;
	insert_handle(&process_handles, p->handle, p);
	unlock(&scheduler_lock);

	spawn(p, preamble, countof(extv), extv);
	return p;
}

thread * spawn(process * par, ulong start, int extz, const qword * extv)
{
	int i;
	qword tvirt;
	qword * ttop;
	qword * rtop = (qword *)(LOW_HALF_TOP - PAGE_SIZE);
	thread * t = kalloc(sizeof(thread));

	tvirt = alloc_pgs(2*PAGE_SIZE, VIRT_PAGES);
	t->stack  = allocate(tvirt + 2*PAGE_SIZE, 2*PAGE_SIZE, 0);
	t->state  = STATE_SETUP;
	t->parent = par;
	t->rsp    = (qword)&rtop[-5 - extz];

	swapin(t->stack, 3);

	ttop = (qword *)(tvirt + PAGE_SIZE);
	ttop[-5 - extz] = start;
	ttop[-4 - extz] = USER_CS | 3;
	ttop[-3 - extz] = getfl();
	ttop[-2 - extz] = (qword)(rtop - extz);
	ttop[-1 - extz] = USER_DS | 3;

	for (i = 0; i < extz; i++) {
		ttop[-1 - i] = extv[i];
	}

	swapout(t->stack);
	shift_rgn(t->stack, LOW_HALF_TOP);
	free_pgs(tvirt, 2*PAGE_SIZE, VIRT_PAGES);

	getlock(&scheduler_lock);

	if (head_thrd == NULL) {
		t->next_sched = t;
		head_thrd = t;
	} else {
		t->next_sched = head_thrd->next_sched;
		head_thrd->next_sched = t;
	}

	t->handle = next_thandle++;
	insert_handle(&thread_handles, t->handle, t);
	unlock(&scheduler_lock);

	t->next_inproc = par->first;
	par->first = t;
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
