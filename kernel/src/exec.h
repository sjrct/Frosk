//
// exec.h
//

#ifndef EXEC_H
#define EXEC_H

#include "scheduler.h"

process * bexec(const byte *, ulong, int, const char **);
process * fexec(const char *, int, const char **);

#endif
