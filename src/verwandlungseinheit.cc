#include <sonderfalle.h>
#include <trace.h>
#include "verwandlungseinheit.h"

using namespace std;
using namespace kunstspeicher;

verwandlungseinheit::verwandlungseinheit(wahrspeicher &s):
	hs(s) {}

template <class type> void verwandlungseinheit::s(h32 k, h32 g, type a) {
	h32 w[sizeof a];
	this->vwl(w, k, sizeof a, g, zugriff::s);
	for (size_t i(0); i < sizeof a; i++)
		this->hs.s(w[i], static_cast<h8>(a >> (((sizeof a) - i - 1)*CHAR_BIT)));
}
template <> void verwandlungseinheit::s(h32 k, h32 g, h8 a) {
	h32 w;
	this->vwl(&w, k, 1, g, zugriff::s);
	this->hs.s(w, a);
}
template void verwandlungseinheit::s(h32 k, h32 g, h8 a);
template void verwandlungseinheit::s(h32 k, h32 g, h16 a);
template void verwandlungseinheit::s(h32 k, h32 g, h32 a);
template void verwandlungseinheit::s(h32 k, h32 g, h64 a);

template <class type> void verwandlungseinheit::l(type &a, h32 k, h32 g) {
	h32 w[sizeof a];
	this->vwl(w, k, sizeof a, g, zugriff::l);
	h8 b;
	a = 0;
	for (size_t i(0); i < sizeof a; i++) {
		this->hs.l(b, w[i]);
		a |= static_cast<type>(b) << (((sizeof a) - i - 1)*CHAR_BIT);
	}
}
template <> void verwandlungseinheit::l(h8 &a, h32 k, h32 g) {
	h32 w;
	this->vwl(&w, k, 1, g, zugriff::l);
	this->hs.l(a, w);
}
template void verwandlungseinheit::l(h8 &a, h32 k, h32 g);
template void verwandlungseinheit::l(h16 &a, h32 k, h32 g);
template void verwandlungseinheit::l(h32 &a, h32 k, h32 g);
template void verwandlungseinheit::l(h64 &a, h32 k, h32 g);

template <class type> void verwandlungseinheit::a(type &a, h32 k, h32 g) {
	h32 w[sizeof a];
	this->vwl(w, k, sizeof a, g, zugriff::a);
	h8 b;
	a = 0;
	for (size_t i(0); i < sizeof a; i++) {
		this->hs.l(b, w[i]);
		a |= static_cast<type>(b) << (((sizeof a) - i - 1)*CHAR_BIT);
	}
}
template <> void verwandlungseinheit::a(h8 &a, h32 k, h32 g) {
	h32 w;
	this->vwl(&w, k, 1, g, zugriff::a);
	this->hs.l(a, w);
}
template void verwandlungseinheit::a(h8 &a, h32 k, h32 g);
template void verwandlungseinheit::a(h16 &a, h32 k, h32 g);
template void verwandlungseinheit::a(h32 &a, h32 k, h32 g);
template void verwandlungseinheit::a(h64 &a, h32 k, h32 g);

void verwandlungseinheit::g(h64 &a, h32 k, h32 g) {
	h32 w[sizeof a];
	this->vwl(w, k, sizeof a, g, zugriff::g);
	h8 b;
	for (size_t i(0); i < sizeof a; i++) {
		this->hs.l(b, w[i]);
		a |= static_cast<h64>(b) << (((sizeof a) - i - 1)*CHAR_BIT);
	}
}

void verwandlungseinheit::enk(h32 k, h32 g) {
	umstand z(g, ((ka::gz & k) >> 22));
	if (this->s2f.find(z) == this->s2f.end())
		return;
	auto e(this->s2f[z]);
	if (!fb::r(e)) {
		this->s2f.erase(z);
		return;
	}

	z = umstand(s1::z & e, ((ka::s1z & k) >> 12));
	if (this->s1f.find(z) == this->s1f.end())
		return;
	this->s1f.erase(z);
}

void verwandlungseinheit::vwl(h32 *w, h32 k, size_t l, h32 g, zugriff z) {
	for (size_t i(0); i < l; i++) {
		auto ki(k + i);
		// Grundfeldlauf
		umstand le(g, ((ka::gz & ki) >> 22));
		if (this->s2f.find(le) == this->s2f.end()) {
			h32 f;
			this->hs.l(f, le.first + (le.second << 2));
			if (!(ss::r(f) || fb::r(f) || sb::s2(f)))
				throw ZEE(ki, g);
			this->s2f[le] = f;
		}
		auto e(this->s2f[le]);
		if (ss::r(e) || sb::s2(e)) {
			w[i] = this->s2zg(e, z, ki, le.first);
			continue;
		}

		// 1-Seitefeldlauf
		le = umstand(s1::z & e, ((ka::s1z & ki) >> 12));
		if (this->s1f.find(le) == this->s1f.end()) {
			h32 f;
			this->hs.l(f, le.first + (le.second << 2));
			if (!(ss::r(f) || sb::s1(f)))
				throw ZEE(ki, g);
			this->s1f[le] = f;
		}
		e = this->s1f[le];
		{
			w[i] = this->s1zg(e, z, ki, le.first);
		}
	}
}

h32 verwandlungseinheit::s2zg(h32 e, zugriff z, h32 k, h32 g) {
	int l((e & (ss::s | ss::l | ss::a)) >> 1), za(static_cast<int>(z));
	if (!((za & l) || (!za & !l)))
		throw ZZE(k, g, z, l);
	h32 w;
	if (ss::r(e)) {
		w = ss::z & e;
		this->hs.l(e, w);
	} else {
		w = (s1::z & g) + ((ka::gz & k) >> (32 - 10 - 2));
	}
	if (st::w & e)
		throw ZSW(k, g, w);
	if (z == zugriff::s)
		this->hs.s(w, e | st::a);
	return (s2::z & e) + (s2::a & k);
}

h32 verwandlungseinheit::s1zg(h32 e, zugriff z, h32 k, h32 f) {
	int l((e & (ss::s | ss::l | ss::a)) >> 1), za(static_cast<int>(z));
	if (!((za & l) || (!za & !l)))
		throw ZZE(k, f, z, l);
	h32 w;
	if (ss::r(e)) {
		w = ss::z & e;
		this->hs.l(e, w);
	} else {
		w = (s1::z & f) + ((ka::s1z & k) >> (32 - 20 - 2));
	}
	if (st::w & e)
		throw ZSW(k, f, w);
	if (z == zugriff::s)
		this->hs.s(w, e | st::a);
	return (s1::z & e) + (s1::a & k);
}
