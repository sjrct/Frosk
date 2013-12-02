//
// include/lib/lib.h
// include/prgm/prgm.h
// include/util/util.h
// include/kernel/util.h
//

#ifndef _UTIL_H_
#define _UTIL_H_

#include <types.h>
#include <stddef.h>

#ifdef __cplusplus
	#define STRUCT(X) struct X
	#define UNION(X)  union  X
	#define ENUM(X)   enum   X
#else
	#define STRUCT(X) struct X; typedef struct X X; struct X
	#define UNION(X)  union  X; typedef union  X X; union  X
	#define ENUM(X)   enum   X; typedef enum   X X; enum   X
#endif

#define at(ADDR, TYPE) (* (TYPE *) ((ulong)(ADDR)))
#define atb(X) at((X), byte)
#define atw(X) at((X), word)
#define atd(X) at((X), dword)
#define atq(X) at((X), qword)

#define align(X, TO) ((X) % (TO) ? (X) - ((X) % (TO)) + (TO) : (X))
#define ceil(X, TO) align(X, TO)
#define floor(X, TO) ((X) % (TO) ? (X) - ((X) % (TO)) : (X))

#define access(FROM,FLD,DEF) ((FROM) == NULL ? (DEF) : (FROM)->FLD)

#define LB(X) (BAT(X, 0))
#define HB(X) (BAT(X, 1))
#define BAT(X,I) (((X) >> (I)*8) & 0xff)

#endif
