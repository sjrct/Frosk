//
// lib/extra/src/string.c
// kernel/src/extra/string.c
//

#include <extra/string.h>

size_t strchri(const char * str, char delim)
{
	int i;
	for (i = 0; str[i] != 0; i++) {
		if (str[i] == delim) return i;
	}
	return i;
}
