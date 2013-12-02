//
// util/fsb/src/fs.h
//

#ifndef FS_H
#define FS_H

#include <util.h>
#include <stdio.h>
#include <fs/node.h>

STRUCT(fs_t);
STRUCT(node);
STRUCT(meta);

struct node {
	fnode_type type;
	uint size;
	fptr ptr;
	meta * location;
	node * next;
	node * prev;
	
	union {
		struct {
			node * next;
			node * prev;
		} free;
	
		struct {
			word perm;
			node * name;
			node * owner;
			node * next;
			node * data;
		} common;
		
		struct {
			word size;
			node * next;
			node * left;
			node * right;
			char * value;
		} string;
		
		struct {
			char * filename;
			node * next;
			qword lba;
			qword size;
		} block;
		
		struct {
			meta * sub;
		} branch;
	} u;
};

struct meta {
	qword lba;
	fptr base;
	uint shift;
	uint subs;
	node * first;
};

struct fs_t {
	uint block_size;
	qword next_lba;
	meta * first_meta;
	node * root;
	node * free_data;
	node * free_meta;
	node * strtbl;
};

#define ENTRY_SIZE (fs.block_size / 0x100)

extern fs_t fs;

meta * new_meta(node *);
node * new_node(uint);
node * new_file (const char *, const char *, node *);
node * new_dir  (const char *, node *);
node * new_indir(const char *, const char *, node *);
node * get_string(const char *);

void write_fs(FILE *);
void debug_fs(FILE *);

#endif
