//
// kernel/src/handle_set.h
//

#ifndef HANDLE_SET_H
#define HANDLE_SET_H

#include <util.h>
#include <frosk/def.h>

STRUCT(handle_set) {
	handle_t hndl;
	void * data;
	handle_set * left;
	handle_set * right;
};

void insert_handle(handle_set **, handle_t, void *);
void * handle_lookup(handle_set *, handle_t);
void * remove_handle(handle_set **, handle_t);

#endif
