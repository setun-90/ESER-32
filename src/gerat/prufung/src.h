#ifndef   _PG_H
#define   _PG_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* ESER Prüfungsgerätsbegriffserklärung */
/*        (C) 1979, 1988, 1992          */

#include <platform.h>
#include "../../durchgangeinheit.h"

#include <random>

struct prufung: public durchgangeinheit {
	using durchgangeinheit::durchgangeinheit;

	h32 operator()(h64 a) override;
	void g_s(h32 z, h32 ab) override;
	void g_l(h32 z, h32 ab) override;

private:
	std::mt19937_64 e;
};

GERAT durchgangeinheit *abb(wahrspeicher &hs, std::istringstream &i);
GERAT void zes(durchgangeinheit *d);


#endif /* _PG_H */
