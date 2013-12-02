//
// kernel/src/dev/disk/ata.h
//
// Current limitations ata driver:
//   28 bit lba
//   8 bit sector count
//

#ifndef IO_DISK_ATA_H
#define IO_DISK_ATA_H

#include <util.h>
#include <types.h>
#include "../dev.h"

STRUCT(ata_source) {
	ushort base_port;
	uchar  is_slave;
};

void detect_ata(void);

void ata_enable(device_t *);
ulong ata_read (device_t *, void *, ulong, ulong);
ulong ata_write(device_t *, void *, ulong, ulong);

#endif
