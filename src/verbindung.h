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

#include <string>

#if defined(ZUSE_POSIX)
#include <dlfcn.h>
using buchse = void *;
#elif defined(ZUSE_WINDOWS)
#include <Windows.h>
using name = LPCSTR;
#endif

struct durchgangeinheit::verbindung {
	explicit verbindung(std::string n);
	void zs(void);

	~verbindung();
	verbindung();
	verbindung(verbindung &&);
	verbindung &operator=(verbindung &&);

	std::shared_ptr<durchgangeinheit> abb(wahrspeicher &hs, std::istringstream &i);

private:
	std::string n;
	buchse b;
	std::shared_ptr<durchgangeinheit> (*a)(wahrspeicher &hs, std::istringstream &i);
};



#endif /* ZUSE_VB_H */
