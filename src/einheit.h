#ifndef   _VE_H
#define   _VE_H



// Copyright 2020 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Einheitsentwurf */
/* (C) 1979, 1988, 1992 */

#include <hauptspeicher.h>
#include <unterbrechung.h>
#include "verwandlungseinheit.h"

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

struct einheit {
	virtual ~einheit() = default;
	virtual bool ls(void) = 0;
	void ub(h64 e);
	void an(void);
	void ab(void);
	static h64 const nube = static_cast<h64>(1) << 32;

protected:
	verwandlungseinheit se;
	virtual void operator()(void) = 0;
	std::atomic<bool> ss;
	std::atomic<h64> ube;
	std::thread t;
	std::condition_variable cv;
	std::mutex m;

	einheit(wahrspeicher &e);
	einheit(einheit const &) = delete;
	einheit &operator=(einheit const &) = delete;
	einheit(einheit &&) = default;
	einheit &operator=(einheit &&) = default;
};

h16 vzw(h16 n, h8 p);
h32 vzw(h32 n, h8 p);
h64 vzw(h64 n, h8 p);



#endif /* _VE_H */
