//
// include/kernel/frfs.h
// include/util/frfs.h
//
// written by sjrct
//

#ifndef _FRFS_H_
#define _FRFS_H_

#define FS_FREE  0
#define FS_FILE  1
#define FS_DIR   2
#define FS_INDIR 3
#define FS_BLOCK 4
#define FS_STR   5
#define FS_BRCH  6
#define FS_HDR   'F'

#define FS_NPMB 128
#define FS_ND_STR_SZ 22

#pragma pack(push, 1)
typedef struct {
	char type;
	
	union {
		struct {
			char pack[3];
			unsigned next;
			char pack2[24];
		} free;
		
		struct {
			char perm;
			char pack[2];
			unsigned name;
			unsigned owner;
			unsigned next;
			unsigned data;
			char pack2[12];
		} cmn;
		
		struct {
			char size;
			char str[FS_ND_STR_SZ];
			unsigned next_part;
			unsigned next_tbl;
		} str;
		
		struct {
			char pack[3];
			unsigned next;
			unsigned long lba;
			unsigned long size;
			char pack2[8];
		} block;
		
		struct {
			char subs;
			char pack[6];
			unsigned long lba1;
			unsigned long lba2;
			char pack2[8];
		} brch;
		
		struct {
			char mark[7];
			unsigned root;
			unsigned kernel;
			unsigned free_blocks;
			unsigned free_nodes;
			unsigned string_table;
			char pack[4];
		} hdr;
	} u;
} fs_node;
#pragma pack(pop)

#endif
