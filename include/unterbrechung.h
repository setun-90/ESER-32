#ifndef   _UB_H
#define   _UB_H



/* EZER Unterbrechung Begriffserklärungen */
/*          (C) 1979, 1989, 1992          */

#include "kunstspeicher.h"

namespace unterbrechung {
static inline h32 g(h64 e) { return (h32)(e >> 44); }
static inline h32 z(h64 e) { return (h32)(e & 0x00000000FFFFFFFF); }
}


#endif /* _UB_H */
