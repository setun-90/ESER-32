#ifndef   _KS_H
#define   _KS_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Kunstspeicher Begriffserklärungen */
/*          (C) 1979, 1988, 1992          */

#include "hauptspeicher.h"

#include <unordered_map>

namespace kunstspeicher {
enum class zugriff {
	s = 0x4, // 0b100
	l = 0x2, // 0b010
	a = 0x1, // 0b001
	g = 0    // 0b000
};

static std::unordered_map<zugriff, char> zugriff_wb({
	{zugriff::s, 's'},
	{zugriff::l, 'l'},
	{zugriff::a, 'a'},
	{zugriff::g, 'g'}
});

/* Angaben */
/*** Kunstanschriften */
namespace ka {
	h32 const  gz(0xFFC00000u);
	h32 const s1z(0x003FF000u);
}

/*** Schlüsseln */
namespace ss {
	static inline bool r(h32 e) { return e & 0x00000001u; }
	h32 const z(0xFFFFFFF0u);
	h32 const s(0x00000008u);
	h32 const l(0x00000004u);
	h32 const a(0x00000002u);
}
/*** Seitebeschreibungen */
namespace sb {
	static inline bool s1(h32 e) { return !ss::r(e) && (e & 0x00000800u); }
	static inline bool s2(h32 e) { return !s1(e)    && (e & 0x00200000u); }
}
/*** Tore */
namespace st {
	h32 const a(0x00000020u);
	h32 const w(0x00000010u);
}
/*** Feldbeschreibungen */
namespace fb {
	static inline bool r(h32 e) { return !ss::r(e) && (e & 0x00000800u); }
	h32 const z(0xFFFFF000u);
}

/*** Zeiger- und Abstandsarten */
namespace s1 {
	h32 const a(0x00000FFFu);
	h32 const z(0xFFFFF000u);
}
namespace s2 {
	h32 const a(0x003FFFFFu);
	h32 const z(0xFFC00000u);
}
}



#endif /* _KS_H */
