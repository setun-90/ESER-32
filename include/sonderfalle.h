#ifndef   ESER_SF_H
#define   ESER_SF_H



// Copyright 2020, 2022- Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* ESER Sonderfalle Begriffserklärungen */
/*         (C) 1979, 1988, 1992         */

#include "kunstspeicher.h"

#include <stdexcept>

class sonderfalle: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class ZEE: public sonderfalle {
	h32 _k, _f;
public:
	ZEE(h32 k, h32 f);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
};
class ZZE: public sonderfalle {
	h32 _k, _f;
	kunstspeicher::zugriff _e;
	int _l;
public:
	ZZE(h32 k, h32 f, kunstspeicher::zugriff e, int l);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	kunstspeicher::zugriff e(void) { return this->_e; }
	h32 l(void) { return this->_l; }
};
class ZSW: public sonderfalle {
	h32 _k, _f, _w;
public:
	ZSW(h32 k, h32 f, h32 w);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	h32 w(void) { return this->_w; }
};
class AUA: public sonderfalle {
	h32 _k, _f;
	h64 _a;
public:
	AUA(h32 k, h32 f, h64 a);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	h64 a(void) { return this->_a; }
};



#endif /* ESER_SF_H */
