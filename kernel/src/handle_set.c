//
// kernel/src/handle_set.c
//

#include "debug.h"
#include "handle_set.h"
#include "memory/kernel.h"

static handle_set * create_node(handle_t hndl, void * data)
{
	handle_set * s = kalloc(sizeof(handle_set));

	s->data = data;
	s->hndl = hndl;
	s->left = NULL;
	s->right = NULL;

	return s;
}

static handle_set ** find(handle_set ** ptr, handle_t hndl)
{
	while (*ptr != NULL) {
		if ((*ptr)->hndl < hndl) {
			ptr = &(*ptr)->left;
		} else if ((*ptr)->hndl > hndl) {
			ptr = &(*ptr)->right;
		} else break;
	}

	return ptr;
}

void insert_handle(handle_set ** ptr, handle_t hndl, void * data)
{
	ptr = find(ptr, hndl);
	if (*ptr != NULL) return;
	*ptr =  create_node(hndl, data);
}

void * handle_lookup(handle_set * set, handle_t hndl)
{
	handle_set ** np = find(&set, hndl);
	if (*np == NULL) return NULL;
	return (*np)->data;
}

static handle_set ** rightmost(handle_set ** n)
{
	if ((*n)->right == NULL) return n;
	return rightmost(&(*n)->right);
}

static void remove(handle_set ** ptr)
{
	handle_set ** prep;
	handle_set * n, * rep;

	n = *ptr;
	
	if (n != NULL) {
		if (n->left == NULL && n->right == NULL) {
			*ptr = NULL;
		} else if (n->left == NULL) {
			*ptr = n->right;
		} else if (n->right == NULL) {
			*ptr = n->left;
		} else {
			prep = rightmost(&n->left);
			rep = *prep;
			*prep = rep->left;
			rep->left = n->left;
			rep->right = n->right;
			*ptr = rep;
		}
	}
}

void * remove_handle(handle_set ** ptr, handle_t hndl)
{
	void * ret;
	handle_set * n;

	ptr = find(ptr, hndl);

	if (*ptr != NULL) {
		n = *ptr;
		ret = n->data;
		remove(ptr);
		kfree(n);
	} else {
		ret = NULL;
	}

	return ret;
}