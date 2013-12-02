//
// kernel/src/file.c
//

#include <common.h>
#include <string.h>
#include <extra/string.h>
#include "file.h"
#include "../io.h"
#include "../debug.h"
#include "../paging.h"
#include "../dev/dev.h"
#include "../memory/pages.h"
#include "../memory/kernel.h"

#define HEADER (*(fmeta_header *)(root_mdb.addr))

STRUCT(mdb) {
	qword lba;
	char * addr;
	mdb * next, * sub;
};

static mdb root_mdb;
static fnode * root_dir;
static int spb;
static device_t * root_dev;

static void estab_mdb(mdb * m)
{
	fnode * n = (fnode *)m->addr;
	
	while (1) {
		if (n->type == FNODE_BRANCH) {
			n->u.branch.os_use = 0;
		}
		
		if (n->next == 0) break;
		n = (fnode *)(m->addr + n->next * 0x10);
	}
}

static fnode * resolve(uint ptr)
{
	int depth = 0;
	ulong virt, phys;
	fnode * fnd = NULL;
	mdb * m = &root_mdb;
	
	if (ptr != 0) {
		fnd = (fnode *)(m->addr + (ptr & 0xff) * 0x10);
		
		while (fnd->type == FNODE_BRANCH && depth < 4) {
			if (fnd->u.branch.os_use == 0) {
				virt = alloc_pgs(PAGE_SIZE, VIRT_PAGES);
				phys = alloc_pgs(PAGE_SIZE, PHYS_PAGES);
				pageto(virt, phys | 3);

				read(root_dev, (void *)phys, spb * fnd->u.branch.lba1, HEADER.page_size);

				fnd->u.branch.os_use = virt;
			}
			
			m = (mdb *)fnd->u.branch.os_use;
			ptr >>= 8;
			fnd = (fnode *)(m->addr + (ptr & 0xff) * 0x10);
			depth++;
		}
		
		assert(depth < 4);
	}
	
	return fnd;
}

void setup_fs(void)
{
	handle_t h = dev_of(DEVICE_ROOT_DISK);
	assert(h != INVALID_HANDLE);

	root_dev = get_dev(h);
	assert(root_dev != NULL);

	root_mdb.addr = (char *)RFSMB_LOC;
	root_mdb.lba  = 1;
	root_mdb.next = NULL;
	root_mdb.sub  = NULL;
	estab_mdb(&root_mdb);

	root_dir = resolve(HEADER.root);
	spb = HEADER.page_size / root_dev->block_size;
}

file_t * fs_aquire(const char * nm)
{
	size_t i;
	fnode * n, * s;
	file_t * f;
	
	n = resolve(root_dir->u.common.data);
	
	while (1) {
		i = strchri(nm, '/');
	
		while (n != NULL) {
			s = resolve(n->u.common.name);
			assert(s->type == FNODE_STRING);
		
			if (!strncmp(nm, FNODE_STRING(s), i)) break;
		
			n = resolve(n->u.common.next);
		}
		
		if (n == NULL) return NULL;
		
		if (nm[i] == '/') {
			n = resolve(n->u.common.data);
			nm += i + 1	;
		} else {
			break;
		}
	}
	
	f = kalloc(sizeof(file_t));
	f->node = n;
	
	return f;
}

void fs_release(file_t * f)
{
	kfree(f);
}

ulong fs_read(byte * buf, ulong offset, ulong size, file_t * f)
{
	ulong a, s;
	ulong osize = size;
	fnode * block = resolve(f->node->u.common.data);

	while (block != NULL && size) {
		a = block->u.block.lba * HEADER.page_size;

		if (!offset) {
			s = block->u.block.size;
		} else {
			if (block->u.block.size < offset) {
				s = block->u.block.size - offset;
				a += offset;
				offset = 0;
			} else {
				offset -= block->u.block.size;
				block = resolve(block->u.block.next);
				continue;
			}
		}

		s = size > s ? s : size;
		read(root_dev, buf, a, s);
		size -= s;
		buf += s;
		
		block = resolve(block->u.block.next);
	}

	return osize - size;
}

ulong fs_write(const byte *, ulong, ulong, file_t *);

ulong fs_filesize(file_t * f)
{
	ulong size = 0;
	fnode * n = f->node;

	if (n->type == FNODE_FILE) {
		n = resolve(n->u.common.data);
	
		while (n != NULL) {
			size += n->u.block.size;
			n = resolve(n->u.block.next);
		}
	}
	
	return size;
}
