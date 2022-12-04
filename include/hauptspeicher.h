#ifndef   _HS_H
#define   _HS_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Hauptspeicher Begriffserklärungen */
/*          (C) 1979, 1988, 1992          */

#include <cstdint>
#include <climits>

using h8  = uint8_t;
using h16 = uint16_t;
using h32 = uint32_t;
using h64 = uint64_t;

template <class type> h8 ss1(h8 i) {
	return sizeof(type)*CHAR_BIT - i;
}

template <class type, h8 i> type stelle(type a) {
	return a & (static_cast<type>(1) << ss1<type>(i));
}
template <h8 i> h8 stelle(h8 a) {
	return stelle<h8, i>(a);
}
template <h8 i> h16 stelle(h16 a) {
	return stelle<h16, i>(a);
}
template <h8 i> h32 stelle(h32 a) {
	return stelle<h32, i>(a);
}
template <h8 i> h64 stelle(h64 a) {
	return stelle<h64, i>(a);
}

template <class type, h8 i, h8 j> type feld(type a) {
	return (a >> ss1<type>(j)) & ((static_cast<type>(1) << (j - i)) - 1);
}
template <h8 i, h8 j> h8 feld(h8 a) {
	return feld<h8, i, j>(a);
}
template <h8 i, h8 j> h16 feld(h16 a) {
	return feld<h16, i, j>(a);
}
template <h8 i, h8 j> h32 feld(h32 a) {
	return feld<h32, i, j>(a);
}
template <h8 i, h8 j> h64 feld(h64 a) {
	return feld<h64, i, j>(a);
}



#endif /* _HS_H */
