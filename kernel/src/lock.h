//
// kernel/src/lock.h
//

#ifndef LOCK_H
#define LOCK_H

typedef unsigned char lock;

int trylock(lock *);
void getlock(lock *);
void unlock(lock *);

#define initlock unlock

#define LOCKED   (0)
#define UNLOCKED (-1)

#endif
