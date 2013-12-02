//
// kernel/src/dev/disk/ata.c
//

#include <common.h>
#include <string.h>
#include "ata.h"
#include "../dev.h"
#include "../../debug.h"
#include "../../io.h"
#include "../../asm.h"
#include "../../error.h"
#include "../../leftovers.h"
#include "../../memory/kernel.h"

#define SECTOR_SIZE   0x200
#define BUSY_BIT      0x80
#define READ_COMMAND  0x20
#define WRITE_COMMAND 0x30

static void wait_busy(ata_source * src)
{
	uchar status;
	do {
		status = inb(src->base_port + 7);
	} while (status & BUSY_BIT);
}

static void ata_common(ulong lba, uint blcks, uchar cmd, ata_source * src)
{
	outb(src->base_port + 6, 0xE0 | ((lba >> 24) & 0xf) | src->is_slave);
	outb(src->base_port + 1, 0);
	outb(src->base_port + 2, (uchar)blcks);
	outb(src->base_port + 3, (uchar)lba);
	outb(src->base_port + 4, (uchar)(lba >> 8));
	outb(src->base_port + 5, (uchar)(lba >> 16));

	outb(src->base_port + 7, cmd);

	// delay ~400ns
	io_wait();
	io_wait();
	io_wait();
	io_wait();
}

void detect_ata(void)
{
	static const char * atatypes[] = {
		"master",
		"slave"
	};

	device_t * dev;
	ata_source * src;
	qword edd;

	if (!memcmp(leftovers.dd.if_type, "ATA", 3)) {
		if (leftovers.dd.edd == 0xffffffff) {
			fatal("Enhanced disk drive specification invalid.");
		}
		
		edd = leftovers.dd.edd & 0xffff;
		edd += (leftovers.dd.edd >> 16) * 0x10;
		
		src = kalloc(sizeof(ata_source));
		src->base_port = atw(edd);
		src->is_slave = !!(atb(edd + 4) & 0x10);

		kprintf(
			"Booted from %s ATA device on port %x.\n",
			atatypes[src->is_slave],
			src->base_port
		);

		dev = create_arbitrary(
			leftovers.dd.bps,
			ata_enable,
			NULL,
			ata_read,
			ata_write
		);

		dev->data = src;
		reg_dev(DEVICE_ROOT_DISK, dev->hndl);
	}
}

void ata_enable(device_t * d)
{
	ata_source * src = d->data;
	src->is_slave = !!src->is_slave << 4;
	assert(d->block_size == SECTOR_SIZE);
}

ulong ata_read(device_t * d, void * data, ulong offset, ulong size)
{
	uint i, j;
	ata_source * src = d->data;

	ata_common(offset, size, READ_COMMAND, src);

	for (i = 0; i < size; i++) {
		wait_busy(src);
		for (j = 0; j < 0x100; j++)  {
			((ushort *)data)[i * 0x100 + j] = inw(src->base_port);
		}
	}

	return size;
}

ulong ata_write(device_t * d, void * data, ulong offset, ulong size)
{
	uint i, j;
	ata_source * src = d->data;

	ata_common(offset, size, WRITE_COMMAND, src);

	for (i = 0; i < size; i++) {
		wait_busy(src);
		for (j = 0; j < 0x100; j++)  {
			outw(src->base_port, ((ushort*)data)[i * 0x100 + j]);
		}
	}

	return size;
}

// Maybe actually use this function ?
/*
static int ata_identify(void)
{
	uchar v1, v2, p1, p2;
	
	// check for floating bus
	v1 = inb(active.base_port + 7);
	if (v1 == 0xff) return 0;
	
	// check that is ATA
	p1 = inb(active.base_port + 2);
	p2 = inb(active.base_port + 3);
	
	outb(active.base_port + 2, 0x12); // values are arbitrary
	outb(active.base_port + 3, 0x34);
	v1 = inb(active.base_port + 2);
	v2 = inb(active.base_port + 3);
	
	outb(active.base_port + 2, p1);
	outb(active.base_port + 3, p2);
	
	if (v1 != 0x12 || v2 != 0x34) return 0;
	
	// identify command
	// TODO
	
	return 1;
}
*/
