#ifndef   _WS_H
#define   _WS_H



// Copyright 2020 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Wahrspeicherbegriffserklärung */
/*        (C) 1979, 1988, 1992        */
#include <hauptspeicher.h>

#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstring>
#include <cstddef>

struct einheit;

class wahrspeicher {
	std::vector<h8> hs;
	std::mutex m;
	std::unordered_map<h32, std::shared_ptr<einheit>> ut;

public:
	wahrspeicher(h32 g);

	void ute(h32 a, std::shared_ptr<einheit> e);
	void ute(h32 a);

	template <class type> void s(h32 w, type a);
	template <class type> void l(type &a, h32 w);

	size_t g();
};



#endif /* _WS_H */
