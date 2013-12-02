//
// kernel/src/ipc/port.c
//

#include <string.h>
#include "port.h"
#include "../scheduler.h"
#include "../handle_set.h"
#include "../memory/kernel.h"

#define PACKET_ALIGN 32

STRUCT(packet) {
	uint size;
	uint offset;
	uint free
	uint big_size;
	ubyte * data;
	packet * next;
};

STRUCT(packet_list) {
	handle_t from;
	packet * first;
	packet_list * next;
};

static handle_set * to_set = NULL;

handle_t poll_port(void)
{
	packet_list * ls = handle_lookup(to_set, cur_thrd->handle);
	if (ls == NULL) return INVALID_HANDLE;
	return ls->from;
}

uint read_port(void * data, uint size, handle_t hndl)
{
	packet * pkt;
	packet_list * ls;
	packet_list * pv = NULL;
	uint osize = size;

	ls = handle_lookup(to_set, cur_thrd->handle);

	while (ls != NULL && ls->from != hndl) {
		pv = ls;
		ls = ls->next;
	}

	if (ls != NULL) {
		pkt = ls->first;
		while (size > 0 && pkt != NULL) {
			if (size < pkt->size) {
				memcpy(data, pkt->data + offset, size);

				pkt->size -= size;
				pkt->offset += size;
				size = 0;
			} else {
				memcpy(data, pkt->data + offset, pkt->size);
				kfree(pkt->data);

				ls->first = pkt->next;
				kfree(pkt);
				pkt = ls->first;
			}
		}

		if (pkt->first == NULL) {
			if (pv == NULL) {
				// FIXME: only remove when no messages at all left
				remove_handle(&to_set, cur_thrd->handle);

				if (ls->next != NULL) {
					insert_handle(&to_set, cur_thrd->handle, ls->next);
				}
			} else {
				pv->next = ls->next;
			}

			kfree(ls);
		}
	}

	return osize - size;
}

uint write_port(void * data, uint size, handle_t to)
{
	packet * pkt;
	packet ** ptr;
	packet_list * ls;
	packet_list * pv = NULL;
	uint osize = size;

	ls = handle_lookup(to_set, to);

	if (ls == NULL) {
		ls = kalloc(sizeof(packet_list));
		ls->from = cur_thrd->handle;
		ls->next = ls->first = NULL;
		insert_handle(&to_set, to, ls);
	} else {
		while (ls != NULL && ls->from != hndl) {
			pv = ls;
			ls = ls->next;
		}

		if (ls == NULL) {
			ls = kalloc(sizeof(packet_list));
			ls->from = cur_thrd->handle;
			ls->next = ls->first = NULL;
			pv->next = ls;
		}
	}

	ptr = &ls->first;
	while (*ptr != NULL) {
		*ptr = (*ptr)->next;
	}

	if (*ptr == NULL || (*ptr)->free < size) {
		if ((*ptr)->free < size) {
			ptr = (*ptr)->next;
		}

		pkt = kalloc(sizeof(packet));
		pkt->size = pkt->offset = 0;
		pkt->free = align(size, PACKET_ALIGN);
		pkt->free = pkt->big_size;
		pkt->data = kalloc(ptr->data);
		pkt->next = NULL;
		*ptr = pkt;
	} else {
		pkt = *ptr;
	}

	memcpy(pkt->data + pkt->big_size - pkt->free, data, size);
	pkt->free -= size;

	return size;
}
