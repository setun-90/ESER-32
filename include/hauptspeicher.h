#ifndef   _HS_H
#define   _HS_H



// Copyright 2020, 2022 Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* EZER Hauptspeicher Begriffserklärungen */
/*          (C) 1979, 1988, 1992          */

#include <cstdint>
#include <limits>

using h8  = uint_least8_t;
using h16 = uint_least16_t;
using h32 = uint_least32_t;
using h64 = uint_least64_t;

constexpr h8 operator "" _8(unsigned long long int n) {
	return static_cast<h8>(n);
}
constexpr h16 operator "" _16(unsigned long long int n) {
	return static_cast<h16>(n);
}
constexpr h32 operator "" _32(unsigned long long int n) {
	return static_cast<h32>(n);
}
constexpr h64 operator "" _64(unsigned long long int n) {
	return static_cast<h64>(n);
}

template <class type> constexpr h8 ss1(h8 i) {
	return std::numeric_limits<type>::digits - i;
}

template <class type> constexpr type stelle(type a, h8 i) {
	return a & (static_cast<type>(1) << ss1<type>(i));
}
template <h8 i, class type> constexpr type stelle(type a) {
	return stelle(a, i);
}

template <h8 i, h8 j, class type> constexpr type feld(type a) {
	return (a >> ss1<type>(j)) & (static_cast<type>(-1) >> ss1<type>(j - i + 1));
}



#endif /* _HS_H */
