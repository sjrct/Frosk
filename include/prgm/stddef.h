//
// include/kernel/stddef.h
// include/prgm/stddef.h
// include/lib/stddef.h
//

#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifndef NULL
	#define NULL (0)
#endif

#ifndef _SIZE_T_DEFINED
	typedef unsigned long size_t;
	#define _SIZE_T_DEFINED
#endif

#if !defined(countof) && !defined(_COUNTOF_DEFINED)
	#define countof(A) (sizeof(A) / sizeof((A)[0]))
	#define _COUNOF_DEFINED
#endif

#endif
