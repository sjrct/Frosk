//
// kernel/src/main.c
//

#include "io.h"
#include "asm.h"
#include "tss.h"
#include "exec.h"
#include "debug.h"
#include "paging.h"
#include "syscall.h"
#include "leftovers.h"
#include "interrupt.h"
#include "exceptions.h"
#include "io/file.h"
#include "dev/dev.h"
#include "memory/pages.h"
#include "memory/kernel.h"

static void init_bss(void);
static void init_ro(void);

void kmain(void)
{
	process * p;

	static const char * start_argv[] = {
		"sys/start"
	};

	init_ro();
	init_bss();

	setup_display();
	setup_kernel_memory();
	setup_pages();
	setup_tss();
	setup_ints();
	setup_exceptions();
	setup_paging();
	init_devs();
	setup_fs();
	setup_syscalls();

	if ((p = fexec("sys/start", 1,  start_argv)) == NULL) {
		kputs("Could not execute /sys/start\n");
	}

	start_scheduler();
	sti();
	
	while (1) hlt();
}

static void init_bss(void)
{
	extern char _bss_start[];
	extern char _bss_end[];

	char * x = _bss_start;

	for (; x < _bss_end; x++) {
		*x = 0;
	}
}

static void init_ro(void)
{
	extern char _ro_start[];
	extern char _ro_end[];

	char * x = _ro_start;

	assert(!( (ulong)_ro_start & (PAGE_SIZE-1) ));

	for (; x < _ro_end; x += PAGE_SIZE) {
		pageto((ulong)x, (ulong)x | 1);
	}
}
