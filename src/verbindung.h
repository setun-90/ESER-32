#ifndef   ZUSE_VB_H
#define   ZUSE_VB_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* Zuse Elektra Verbindungbegriffserklärung */
/*      (C) 1993, 1998, 2003, 2011          */

#include <platform.h>
#include "durchgangeinheit.h"

#if defined(ZUSE_POSIX)
#include <dlfcn.h>
using buchse = void *;
#elif defined(ZUSE_WINDOWS)
#include <Windows.h>
using buchse = HMODULE;
#endif

struct durchgangeinheit::verbindung {
	verbindung(wahrspeicher &hs, char const *n, std::istringstream &i);

	~verbindung();
	verbindung(verbindung const &) = default;
	verbindung &operator=(verbindung const &) = default;

	std::shared_ptr<durchgangeinheit> ab(void);
	void zs(void);

private:
	buchse b;
	std::shared_ptr<durchgangeinheit> d;
};



#endif /* ZUSE_VB_H */