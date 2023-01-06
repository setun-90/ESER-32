#include "posix.h"

#include <string>
#include <iomanip>

#include <cstring>
#include <cerrno>

using namespace std;
using namespace kunstspeicher;



posix::posix(wahrspeicher &hs, posix::buf f):
	durchgangeinheit(hs), f(move(f)) {}

durchgangeinheit *abb(wahrspeicher &hs, istringstream &i) {
	string n;
	i >> n;
	posix::buf f;
	if (!f.open(n.c_str(), ios::binary | ios::in | ios::out | ios::app)) {
		return nullptr;
	}

	return new posix(hs, move(f));
}
void zes(durchgangeinheit *d) {
	delete d;
}

h32 posix::operator()(h64 a) {
	auto r(stelle<9>(a) ? ios_base::in : ios_base::out);
	if (stelle<10>(a)) {
		/*
		Position test :
		                   1                   2                   3
		 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
		|*|*|0|0|0|A|Z|1|r|0|a a a a a a a a a a a a a a a a a a a a a a|
		|z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z|
		r direction
			0 : input
			1 : output
		a device offset
		z memory offset
		A device offset meaning
			0 : relative
			1 : absolute
		Z memory offset meaning
			0 : immediate
			1 : indirect
		*/
		buf::pos_type o(this->f.pubseekoff(0, ios_base::cur, r));
		h32 ab(vzw<ss1<h32>(11)>(feld<11,32, h64>(a)));
		auto e(stelle<6>(a) ? ios_base::beg : ios_base::cur);
		if (o == this->f.pubseekoff(ab, e, r)) {
			return 8;
		}
		h32 z(feld<33,64>(a));
		if (stelle<7>(a)) {
			h32 zz;
			if (!this->l(zz, this->az + z))
				return 0;
			z = zz;
		}
		return z + 8;
	} else {
		/*
		Position assignment (seek) :
		|*|*|0|0|0|A|G|1|r|1|a a a a a a a a a a a a a a a a a a a a a a|
		r direction
			0 : input
			1 : output
		a memory offset
		A device offset meaning
			0 : relative
			1 : absolute
		G memory offset meaning
			0 : immediate
			1 : indirect
		*/
		auto e(stelle<6>(a) ? ios_base::beg : ios_base::cur);
		h32 ab;
		if (!this->l(ab, this->az + vzw<ss1<h32>(11)>(feld<11,32, h64>(a))))
			return 0;
		this->f.pubseekpos(this->f.pubseekoff(ab, e, r));
		return 4;
	}
}
void posix::g_s(h32 z, h32 ab) {
	/*
	h32 r(z%8), l(z - r), i(ab);
	while (i < ab + l) {
		h64 a;
		if (this->f.sgetn(reinterpret_cast<char const *>(&a), sizeof a) < sizeof a)
			throw runtime_error("Underflow");
		this->se.s(i, a);
		i += 8;
	}
	r = r%4; l = z - r;
	while (i < ab + l) {
		h32 a;
		if (this->f.sgetn(reinterpret_cast<char const *>(&a), sizeof a) < sizeof a)
			throw runtime_error("Underflow");
		this->se.s(i, a);
		i += 4;
	}
	r = r%2; l = z - r;
	while (i < ab + l) {
		h16 a;
		if (this->f.sgetn(reinterpret_cast<char const *>(&a), sizeof a) < sizeof a)
			throw runtime_error("Underflow");
		this->se.s(i, a);
		i += 2;
	}
	if (r) {
		h8 a(this->f.sbumpc());
		if (a == buf::traits_type::eof())
			throw runtime_error("Underflow");
		this->se.s(i + 1, a);
	}
	*/
	/*
	h32 lz(z & (~z + 1));
	h8 a[lz];
	for (h32 i(0); i < lz; i += 1) {
		this->f.read(a);
		d->s(ab + i, a);
	}
	*/
	h8 a;
	for (h32 i(0); i < z; i += 1) {
		a = this->f.sbumpc();
		if (buf::traits_type::to_int_type(a) == buf::traits_type::eof()) {
			this->sf = make_exception_ptr(runtime_error("Underflow"));
			return;
		}
		if (!this->s(ab + i, a))
			return;
	}
}
void posix::g_l(h32 z, h32 ab) {
	/*
	h32 r(z%8), l(z - r), i(ab);
	while (i < ab + l) {
		h64 a;
		this->se.l(a, i);
		if (this->f.sputn(reinterpret_cast<char const *>(&a), sizeof a) == buf::traits_type::eof())
			throw runtime_error("Overflow");
		i += 8;
	}
	r = r%4; l = z - r;
	while (i < ab + l) {
		h32 a;
		this->se.l(a, i);
		if (this->f.sputn(reinterpret_cast<char const *>(&a), sizeof a) == buf::traits_type::eof())
			throw runtime_error("Overflow");
		i += 4;
	}
	r = r%2; l = z - r;
	while (i < ab + l) {
		h16 a;
		this->se.l(a, ab + i);
		if (this->f.sputn(reinterpret_cast<char const *>(&a), sizeof a) == buf::traits_type::eof())
			throw runtime_error("Overflow");
		i += 2;
	}
	if (r) {
		h8 a;
		this->se.l(a, i + 1);
		if (this->f.sputc(a) == buf::traits_type::eof())
			throw runtime_error("Overflow");
	}
	*/
	h8 a;
	for (h32 i(0); i < z; i += 1) {
		if (!this->l(a, ab + i))
			return;
		if (this->f.sputc(a) == buf::traits_type::eof()) {
			this->sf = make_exception_ptr(runtime_error("Overflow"));
			return;
		}
	}
}
