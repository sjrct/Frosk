//
// include/kernel/fs/node.h
// include/util/fs/node.h
//

#ifndef _FS_NODE_H_
#define _FS_NODE_H_

#include <util.h>

typedef uint fptr;

STRUCT(fmeta_header);
STRUCT(fnode_free);
STRUCT(fnode_common);
STRUCT(fnode_string);
STRUCT(fnode_block);
STRUCT(fnode_branch);
STRUCT(fnode);
ENUM(fnode_type);

enum fnode_type {
	FNODE_FREE = 0,
	FNODE_FILE,
	FNODE_DIRECTORY,
	FNODE_INDIRECT,
	FNODE_STRING,
	FNODE_BLOCK,
	FNODE_BRANCH,
};

#define FPERM_READ_OWNER  0x01
#define FPERM_READ_ALL    0x03
#define FPERM_WRITE_OWNER 0x04
#define FPERM_WRITE_ALL   0x0c
#define FPERM_EXEC_OWNER  0x10
#define FPERM_EXEC_ALL    0x30

#define FROSK_FS_MARK "Frosk FS"

#define FNODE_ADD_SIZE 2

#define FNODE_STRING(N) ((char *)(N) + FNODE_ADD_SIZE + sizeof(fnode_string))

#pragma pack(push, 1)
struct fmeta_header {
	byte mark[8];
	dword page_size;
	dword flags;
	fptr root;
	fptr free_meta;
	fptr free_data;
	fptr strtbl;
};

struct fnode_free {
	word size;
	fptr next;
};

struct fnode_common {
	word perm;
	fptr name;
	fptr owner;
	fptr next;
	fptr data;
};

struct fnode_string {
	word size;
	fptr next;
	fptr left;
	fptr right;
};

struct fnode_block {
	word pack;
	fptr next;
	qword lba;
	qword size;
};

struct fnode_branch {
	byte subs;
	byte pack[5];
	qword lba1;
	qword lba2;
	qword os_use;
};

struct fnode {
	byte type;
	byte next;
	
	union {
		fnode_free   free;
		fnode_common common;
		fnode_string string;
		fnode_block  block;
		fnode_branch branch;
	} u;
};
#pragma pack(pop)

#endif
