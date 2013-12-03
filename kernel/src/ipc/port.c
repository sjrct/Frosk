//
// kernel/src/ipc/port.c
//

#include <util.h>
#include <string.h>
#include <frosk/def.h>
#include "port.h"
#include "../scheduler.h"
#include "../handle_set.h"
#include "../memory/kernel.h"

#define PACKET_ALIGN 32

STRUCT(packet) {
	uint size;
	uint big_size;
	uint offset;
	uint free;
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
	packet_list * ls = handle_lookup(&to_set, cur_thrd->handle);
	if (ls == NULL) return INVALID_HANDLE;
	return ls->from;
}

static void find_pl(packet_list * ppv, packet_list ** pls, handle_t hndl)
{
 	while (*pls != NULL && (*pls)->from != hndl) {
		*ppv = *pls;
		*pls = (*pls)->next;
	}
}

static void fix_first(packet_list * pv, packet_list * ls)
{
	if (ls->first == NULL) {
		if (pv == NULL) {
			// FIXME: only remove when no messages left at all
			remove_handle(&to_set, cur_thrd->handle);

			if (ls->next != NULL) {
				insert_handle(&to_set, cur_thrd->handle, ls->next);
			}
		} else {
			pv->next = pk->next;
		}

		kfree(ls);
	}
}

uint read_port(void * data, uint size, handle_t hndl)
{
	packet * pkt;
	packet_list * ls;
	packet_list * pv = NULL;
	uint osize = size;

	ls = handle_lookup(&to_set, cur_thrd->handle);
    find_pl(&pv, &ls, hndl);

	if (ls != NULL) {
		pkt = ls->first;
		while (size > 0 && pkt != NULL) {
			if (size < pkt->size) {
				memcpy(data, pkt->data + pkt->offset, size);

				pkt->size -= size;
				pkt->offset += size;
				size = 0;
			} else {
				memcpy(data, pkt->data + pkt->offset, pkt->size);

				size -= pkt->size;

				ls->first = pkt->next;
				kfree(pkt->data);
				kfree(pkt);
				pkt = ls->first;
			}
		}

		fix_first(pv, ls);
	}

	return osize - size;
}

void refuse_port(handle_t hndl)
{
	packet * p;
	packet_list * ls;
	packet_list * pv = NULL;

	ls = handle_lookup(to_set, cur_thrd->handle);
	find_pl(&pv, &ls, hndl);

	if (ls != NULL) {
		while (ls->first != NULL) {
			p = ls->first;
			ls->first = ls->first->next;
			free(p->data);
			free(p);
		}

		fix_first(pv, ls);
	}
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
		s->from = cur_thrd->handle;
		ls->next = ls->first = NULL;
		insert_handle(&to_set, to, ls);
	} else {
		find_pl(&pv, &ls, to);

		if (ls == NULL) {
			assert(pv != NULL);
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
		pkt->big_size = align(size, PACKET_ALIGN);
		pkt->free = pkt->big_size;
		pkt->data = kalloc(ptr->big_size);
		pkt->next = NULL;
		*ptr = pkt;
	} else {
		pkt = *ptr;
	}

	memcpy(pkt->data + pkt->big_size - pkt->free, data, size);
	pkt->free -= size;

	return size;
}

