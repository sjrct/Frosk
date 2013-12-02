//
// include/prgm/stdint.h
// include/lib/stdint.h
//

#ifndef _STDINT_H_
#define _STDINT_H_

typedef   signed char      int8_t;
typedef   signed short     int16_t;
typedef   signed int       int32_t;
typedef   signed long long int64_t
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef  int8_t   int_least8_t;
typedef  int16_t  int_least16_t;
typedef  int32_t  int_least32_t;
typedef  int64_t  int_least64_t;
typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef  int8_t   int_fast8_t;
typedef  int16_t  int_fast16_t;
typedef  int32_t  int_fast32_t;
typedef  int64_t  int_fast64_t;
typedef uint8_t  uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef  int64_t  intptr_t;
typedef uint64_t uintptr_t;

typedef  int64_t  intmax_t;
typedef uint64_t uintmax_t;

#define INT8_MIN   0xff
#define INT8_MAX   0x7f
#define INT16_MIN  0xffff
#define INT16_MAX  0x7fff
#define INT32_MIN  0xffffffff
#define INT32_MAX  0x7fffffff
#define INT64_MIN  0xffffffffffffffff
#define INT64_MAX  0x7fffffffffffffff
#define UINT8_MAX  0xff
#define UINT16_MAX 0xffff
#define UINT32_MAX 0xffffffff
#define UINT64_MAX 0xffffffffffffffff

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN   INT8_MIN
#define INT_FAST8_MAX   INT8_MAX
#define INT_FAST16_MIN  INT16_MIN
#define INT_FAST16_MAX  INT16_MAX
#define INT_FAST32_MIN  INT32_MIN
#define INT_FAST32_MAX  INT32_MAX
#define INT_FAST64_MIN  INT64_MIN
#define INT_FAST64_MAX  INT64_MAX
#define UINT_FAST8_MAX  UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define  INTN_C(N)  int_least ## N ## _t
#define UINTN_C(N) uint_least ## N ## _t

#endif
