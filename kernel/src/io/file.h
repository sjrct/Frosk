//
// kernel/src/io/file.h
//

#ifndef IO_FILE_H
#define IO_FILE_H

#include <util.h>
#include <types.h>
#include <fs/node.h>

STRUCT(file_t);

struct file_t {
	fnode * node;
};

void setup_fs(void);

file_t * fs_aquire(const char *);
void fs_release(file_t *);

ulong fs_read(byte *, ulong, ulong, file_t *);
ulong fs_write(const byte *, ulong, ulong, file_t *);
ulong fs_filesize(file_t *);

file_t * fs_create_file (const char *, file_t *);
file_t * fs_create_dir  (const char *, file_t *);
file_t * fs_create_indir(const char *, const char *, file_t *);
int remove_file(file_t *);

#endif
