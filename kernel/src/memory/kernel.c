//
// kernel/src/memory/kernel.c
//

#include <util.h>
#include <common.h>
#include <stddef.h>
#include "pages.h"
#include "kernel.h"
#include "../io.h"
#include "../debug.h"
#include "../paging.h"

#define ADJ_PAGE_SIZE (PAGE_SIZE - (sizeof(chunk)))

typedef struct chunk {
	ulong size;
	struct chunk * next;
} chunk;

chunk * head = NULL;
chunk * staged = NULL;

static ulong free_kernel_size = ADJ_PAGE_SIZE;

void setup_kernel_memory(void)
{
	head = (chunk *)KMEM_START;
	head->size = ADJ_PAGE_SIZE;
	head->next = head;
	
	staged = (chunk *)(KMEM_START + PAGE_SIZE);
	staged->size = ADJ_PAGE_SIZE;
}

void * kalloc(ulong size)
{
	ulong adj_size, l;
	ulong phys, virt;
	chunk * first;
	chunk * prev;
	chunk * ret = NULL;

//	kprintf("kalloc(%p)\n", size);

	size = align(size, sizeof(chunk));

	if (size > ADJ_PAGE_SIZE) {
		// big alloc, get some new pages
		kprintf("kalloc: big alloc of %d\n", size);

		adj_size = align(size + sizeof(chunk), PAGE_SIZE);
		virt = alloc_pgs(adj_size, VIRT_PAGES);
		phys = alloc_pgs(adj_size, PHYS_PAGES);

		for (l = 0; l < adj_size; l += PAGE_SIZE) {
			pageto(virt + l, (phys + l) | KERN_PAGE_FL);
		}

		first = (chunk *)virt;
		first->size = adj_size - sizeof(chunk);
		first->next = head->next->next;
		head->next = first;
		head = first;
	} else if (head != NULL) {
		// Look for free data in list
		prev = head;
		head = head->next;
		first = head;

		while (head->size < size) {
			prev = head;
			head = head->next;

			if (head == first) {
				assert(staged != NULL);
				free_kernel_size += staged->size;

				staged->next = head->next;
				head->next = staged;
				head = staged;
				staged = NULL;
				break;
			}
		}
	} else {
		// add the staged to the list
		assert(staged != NULL);
		free_kernel_size += staged->size;

		prev = head = staged;
		head->next = head;
		staged = NULL;
	}
		
	if (head->size == size) {
		// exact match
		ret = head;

		if (prev == head) {
			head = NULL;
		} else {
			head = head->next;
			prev->next = head;
		}
	} else {
		// larger match
		assert(head->size >= size + sizeof(chunk));

		head->size -= size + sizeof(chunk);
		ret = (chunk *)((char *)(head + 1) + head->size);
		ret->size = size;

		free_kernel_size -= sizeof(chunk);
	}

	free_kernel_size -= size;
//	kprintf("Free kernel size = %p\n", free_kernel_size);

	if (staged == NULL) {
		// replace the stage page
		kprintf("kalloc: needed to use staged\n");

		virt = alloc_pgs(PAGE_SIZE, VIRT_PAGES);
		phys = alloc_pgs(PAGE_SIZE, PHYS_PAGES);
		pageto(virt, phys | KERN_PAGE_FL);

		staged = (chunk *)virt;
		staged->size = ADJ_PAGE_SIZE;
	}
		
	return (void *)(ret + 1);
}

void kfree(void * ptr)
{
	chunk * prev, * it;
	chunk * ck = (chunk *)ptr - 1;

	free_kernel_size += ck->size;
	
	if (head == NULL) {
		head = ck;
		ck->next = ck;
	} else {
		it = head->next;
		prev = head;
	
		do {
			if ((ulong)it == (ulong)ptr + ck->size) {
				prev->next = ck;
				ck->next = it->next;
				ck->size += it->size + sizeof(chunk);
				free_kernel_size += sizeof(chunk);

//				kprintf("~Free kernel size = %p\n", free_kernel_size);
				return;
			}
			
			if ((ulong)ck == (ulong)(it + 1) + it->size) {
				it->size += ck->size + sizeof(chunk);
				free_kernel_size += sizeof(chunk);

//				kprintf("~Free kernel size = %p\n", free_kernel_size);
				return;
			}
			
			prev = it;
			it = it->next;
		} while (prev != head);
		
		ck->next = head->next;
		head->next = ck;
	}

//	kprintf("+Free kernel size = %p\n", free_kernel_size);
}
