#ifndef   _UB_H
#define   _UB_H



/* EZER Unterbrechung Begriffserklärungen */
/*          (C) 1979, 1989, 1992          */

#include "kunstspeicher.h"

namespace unterbrechung {
	static inline h32 g(h64 e) { return static_cast<h32>((e >> 32) & 0xFFFFF000U); }
	static inline h32 z(h64 e) { return static_cast<h32>(e); }
}



#endif /* _UB_H */
