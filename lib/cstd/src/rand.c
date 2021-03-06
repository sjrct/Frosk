//
// lib/cstd/src/rand.c
//

#include <stdlib.h>

static unsigned seed;

void srand(unsigned s)
{
	seed = s;
}

// seems to work well with not repeating and not favoriting
// the algorithim (compliments to myself) is basically:
//  -Xor by constant
//  -rotate by 1 more (or 0 if first, or if last = 32) than last value rotated by
//  -add some constant
unsigned rand(void)
{
	static int rot;

	seed ^= 0xa93517f1;
	seed = (seed >> rot) | (seed << ((sizeof(int) * 8) - rot));
	rot++;
	if (rot > 31) rot = 0; 
	seed += 1231;

	return seed;
}
