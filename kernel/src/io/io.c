//
// kernel/src/io/io.c
//

#include <string.h>
#include "../io.h"
#include "../debug.h"
#include "../error.h"
#include "../leftovers.h"
#include "../memory/kernel.h"

ulong read(device_t * d, byte * to, ulong at, ulong size)
{
	uint offset, i, max;
	byte * buf = kalloc(d->block_size);

	assert(d->type == DEVICE_ARBITRARY);
	
	while (size > 0) {
		offset = at % d->block_size;
		max = size - offset;
		max = max > d->block_size ? d->block_size : max;
		
		d->api.arbitrary.read(d, buf, at / d->block_size, 1);
		
		for (i = offset; i < max ; i++) {
			*to = buf[i];
			to++;
			at++;
			size--;
		}
	}
	
	kfree(buf);
	return size;
}
