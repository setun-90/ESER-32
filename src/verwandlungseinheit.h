#ifndef   _VWE_H
#define   _VWE_H



// Copyright 2020 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Anschriftverwandlungseinheitsbegriffserklärung */
/*                 (C) 1979, 1988, 1992                */

#include <kunstspeicher.h>
#include "wahrspeicher.h"

#include <unordered_map>
#include <utility>
#include <functional>
#include <climits>

using umstand = std::pair<h32, h32>;

template <> struct std::hash<umstand> {
	std::hash<h32> h;
	size_t operator()(umstand const &k) const {
		if (sizeof(size_t) == 8)
			return (static_cast<size_t>(k.first) << 32) | static_cast<size_t>(k.second);
		else
			return 3*h(k.first) + h(k.second);
	}
};

using namespace kunstspeicher;

struct verwandlungseinheit {
	verwandlungseinheit(wahrspeicher &s);

	template <class type> void s(h32 k, h32 g, type a);
	template <class type> void l(type &a, h32 k, h32 g);
	template <class type> void a(type &a, h32 k, h32 g);
	void g(h64 &a, h32 k, h32 g);

	void enk(h32 k, h32 g);

private:
	wahrspeicher &hs;

	std::unordered_map<umstand, h32> s1f, s2f;

	void vwl(h32 *w, h32 k, size_t l, h32 g, zugriff z);
	h32 s2zg(h32 e, zugriff z, h32 k, h32 g);
	h32 s1zg(h32 e, zugriff z, h32 k, h32 g);
};



#endif /* _VWE_H */
