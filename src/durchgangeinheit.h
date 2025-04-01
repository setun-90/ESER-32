#ifndef   _DE_H
#define   _DE_H



// Copyright 2020, 2022- Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* ESER Durchgangeinheitsentwurf */
/*     (C) 1979, 1988, 1992      */

#include "einheit.h"

#include <memory>
#include <sstream>
#include <exception>

class durchgangeinheit: public einheit {
	void operator()(void) override;
	void af(void);

	using einheit::se;

protected:
	h32 az;
	h32 gfb;
	h32 utz;
	h32 uez;
	h8  b;
	bool zs;

	template <class type> bool s(h32 k, type a);
	template <class type> bool l(type &a, h32 k);
	template <class type> bool a(type &a, h32 k);

	virtual h32 operator()(h64 a) = 0;
	virtual void g_l(h32 z, h32 ab) = 0;
	virtual void g_s(h32 z, h32 ab) = 0;

public:
	durchgangeinheit(wahrspeicher &hs);
	virtual ~durchgangeinheit() = default;

	bool ls(void) override;
};



#endif /* _DE_H */
