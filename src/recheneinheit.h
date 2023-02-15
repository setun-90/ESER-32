#ifndef   _RE_H
#define   _RE_H



// Copyright 2020 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* ESER Recheneinheitsentwurf */
/*    (C) 1979, 1988, 1992    */

#include "einheit.h"

#include <utility>
#include <condition_variable>
#include <mutex>

class recheneinheit: public einheit {
	void operator()(void) override;
	void nss(h8 z,  h32 a);
	void nsl(h32 &a, h8 z);
	static h8 b1(h32 a);
	static h8 b2(h32 a);
	static h8 b3(h32 a);
	void af(void);

	h32 ns[16];
	h32 az;
	h32 gfb;
	h8  b;
	bool zs, zes;

public:
	recheneinheit(wahrspeicher &hs);

	bool ls(void) override;
};



#endif /* _RE_H */
