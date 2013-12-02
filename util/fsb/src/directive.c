//
// directive.c
//

#include <stdlib.h>
#include <string.h>
#include "fs.h"
#include "parse.h"
#include "directive.h"

#ifndef countof
	#define countof(X) (sizeof(X) / sizeof(X[0]))
#endif

static void confirm_common(directive * d)
{
	if (d->values[2] != NULL) {
		d->nd->u.common.perm = parse_perm(d->values[2]);
	}

	if (d->values[3] != NULL) {
		d->nd->u.common.owner = get_string(d->values[3]);
	}
}

static int fconfirm(directive * d)
{
	if (d->values[0] == NULL || d->values[1] == NULL) return 0;
	
	d->nd = new_file(d->values[0], d->values[1], d->par);
	confirm_common(d);
	
	return 1;
}

static int dconfirm(directive * d)
{
	source * src;
	
	if (d->values[0] == NULL) return 0;

	d->nd = new_dir(d->values[0], d->par);
	confirm_common(d);
	
	if (d->values[1] != NULL) {
		src = source_string(d->values[1]);
		d->nd = parse_directory(src, d->nd);
		free(src);
	}

	return 1;
}

static int iconfirm(directive * d)
{
	if (d->values[0] == NULL || d->values[1] == NULL) return 0;
	
	d->nd = new_indir(d->values[0], d->values[1], d->par);
	confirm_common(d);
	
	return 1;
}

static char * ffields[] = {
	"name",
	"actual",
	"perm",
	"owner",
};

static char * dfields[] = {
	"name",
	"contents",
	"perm",
	"owner",
};	

static char * ifields[] = {
	"name",
	"target",
	"perm",
	"owner",
};

static directive_class classes[] = {
	{ 'f', fconfirm, countof(ffields), ffields },
	{ 'd', dconfirm, countof(dfields), dfields },
	{ 'i', iconfirm, countof(ifields), ifields },
};

directive * get_directive(int mark, node * par)
{
	uint i;
	int j;
	directive * d;
	
	for (i = 0; i < countof(classes); i++) {
		if (classes[i].mark == mark) {
			d = malloc(sizeof(directive));
			
			d->cls = &classes[i];
			d->par = par;
			d->values = malloc(classes[i].field_count * sizeof(char *));
			d->nd = NULL;
			
			for (j = 0; j < classes[i].field_count; j++) {
				d->values[j] = NULL;
			}
			
			return d;
		}
	}
	
	return NULL;
}

int confirm_directive(directive * d)
{
	int i;
	int r = d->cls->confirm(d);

	for (i = 0; i < d->cls->field_count; i++) {
		free(d->values[i]);
	}
	
	return r;
} 

int set_field(directive * d, const char * what, char * to)
{
	int i;
	
	for (i = 0; i < d->cls->field_count; i++) {
		if (!strcmp(what, d->cls->fields[i])) {
			if (d->values[i] != NULL) {
				free(d->values[i]);
			}
			
			d->values[i] = to;
			return 1;
		}
	}
	
	return 0;
}
