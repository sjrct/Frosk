//
// kernel/src/memory/pages.c
//

#include <util.h>
#include <stddef.h>
#include "pages.h"
#include "kernel.h"
#include "../io.h"
#include "../leftovers.h"

typedef struct pages {
	ulong addr, size;
	struct pages * next;
} pages;

pages * heads[2];

void setup_pages(void)
{
	uint i;
	ulong diff, base, size;
	pages * pg;

	for (i = 0; i < leftovers.mm_size / sizeof(mmr_t); i++) {
		if (leftovers.memory_map[i].type == 1) {
			base = leftovers.memory_map[i].base;
			size = leftovers.memory_map[i].size;
		
			diff = !!(base < DEF_STACK_LOC) * DEF_STACK_LOC;
			if (base % PAGE_SIZE) {
				diff += PAGE_SIZE - base % PAGE_SIZE;
			}
			
			if (size > diff) {
				size -= diff;
				base += diff;
		
				pg = kalloc(sizeof(pages));
				pg->addr = base;
				pg->size = floor(size, PAGE_SIZE);

				kprintf("Physical memory of %p bytes at %p\n", pg->size, pg->addr);
		
				if (heads[PHYS_PAGES] == NULL) {
					pg->next = pg;
					heads[PHYS_PAGES] = pg;
				} else {
					pg->next = heads[PHYS_PAGES]->next;
					heads[PHYS_PAGES]->next = pg;
				}
			}
		}
	}

	pg = kalloc(sizeof(pages));
	pg->addr = HIGH_HALF_BOT;
	pg->size = CANON_SIZE;
	pg->next = pg;
	heads[VIRT_PAGES] = pg;
}

ulong alloc_pgs(ulong size, int type)
{
	ulong ret;
	pages * prev = heads[type];
	pages * pg = prev->next;
	
	size = align(size, PAGE_SIZE);
	
	do {
		if (pg->size >= size) {
			if (pg->size == size) {
				ret = pg->addr;
				prev->next = pg->next;
				kfree(pg);
			} else {
				pg->size -= size;
				ret = pg->addr + pg->size;
			}
			
			return ret;
		}
	
		prev = pg;
		pg = pg->next;
	} while (prev != heads[type]);
	
	kprintf("alloc_pgs(%p, %d) returned INVALID_PAGE.\n", size, type);
	
	return INVALID_PAGE;
}

void free_pgs(ulong addr, ulong size, int type)
{
	pages * prev = heads[type];
	pages * pg = prev->next;
	
	size = floor(size, PAGE_SIZE);
	
	do {
		if (pg->addr + pg->size == addr) {
			pg->size += size;
			return;
		}
		
		if (addr + size == pg->addr) {
			pg->size += size;
			pg->addr = addr;
			return;
		}
	
		prev = pg;
		pg = pg->next;
	} while (prev != heads[type]);
	
	pg = kalloc(sizeof(pages));
	pg->addr = addr;
	pg->size = size;
	pg->next = heads[type]->next;
	heads[type]->next = pg;
}

void dump_pgs(int type)
{
	pages * x = heads[type];
	do {
		kprintf("%p : %p\n", x->addr, x->size);
		x = x->next;
	} while (x != heads[type]);
}
