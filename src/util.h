#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#include "arch.h"

#undef assert
#undef __assert
#define assert(e) \
  ((void) ((e) ? ((void) 0) : __assert (#e, __FILE__, __LINE__)))
#define __assert(e, file, line) \
  ((void) Serial.printf ("%s:%u: failed assertion '%s'\n", file, line, e), \
    abort ())

#define VAR_ARG_MACRO(_1,_2,NAME,...) NAME
#define zre_debug_dump(...) VAR_ARG_MACRO(__VA_ARGS__, zre_debug_dump2, zre_debug_dump1)(__VA_ARGS__)

#define zre_debug_dump1(e) \
  ((void) (Serial.printf("%s = ", #e) && Serial.println(e) ))

#define zre_debug_dump2(e, len) \
  ((void) __zre_debug_dump (#e, e, len))
void __zre_debug_dump (const char *label, const uint8_t *buffer, uint8_t len);

#endif
