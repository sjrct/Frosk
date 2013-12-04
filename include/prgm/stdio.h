//
// include/lib/stdio.h
// include/prgm/stdio.h
//

#ifndef _STDIO_H_
#define _STDIO_H_

#include <stddef.h>
#include <stdarg.h>

struct _file;
typedef struct _file FILE;

extern FILE * stdin;
extern FILE * stdout;
extern FILE * stderr;

#define EOF (-1)

#define SEEK_CUR (0)
#define SEEK_SET (1)
#define SEEK_END (2)

int fputc(int, FILE *);
int fputs(const char *, FILE *);
int puts(const char *);

int vfprintf(FILE *, const char *, va_list);
int fprintf(FILE *, const char *, ...);

#define putc(C,F)  fputc(C,F)
#define putchar(C) fputc(C, stdout)
#define printf(FMT,...) fprintf(stdout, FMT, ##__VA_ARGS__)
#define vprintf(FMT,VA) vfprintf(stdout, FMT, VA)

#endif
