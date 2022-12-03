#include "durchgangeinheit.h"

#include <sonderfalle.h>
#include <unterbrechung.h>

using namespace std;
using namespace kunstspeicher;



durchgangeinheit::durchgangeinheit(wahrspeicher &hs, durchgangeinheit::gerat &&gr):
	einheit(hs), zs(false), g(move(gr)) {}

template <class art> void durchgangeinheit::s(h32 k, art a) {
	this->se.s(k, this->gfb, a);
}
template void durchgangeinheit::s(h32 k, h8 a);
template void durchgangeinheit::s(h32 k, h16 a);
template void durchgangeinheit::s(h32 k, h32 a);
template void durchgangeinheit::s(h32 k, h64 a);
template <class art> void durchgangeinheit::l(art &a, h32 k) {
	this->se.l(a, k, this->gfb);
}
template void durchgangeinheit::l(h8 &a, h32 k);
template void durchgangeinheit::l(h16 &a, h32 k);
template void durchgangeinheit::l(h32 &a, h32 k);
template void durchgangeinheit::l(h64 &a, h32 k);
template <class art> void durchgangeinheit::a(art &a, h32 k) {
	this->se.a(a, k, this->gfb);
}
template void durchgangeinheit::a(h32 &a, h32 k);
template void durchgangeinheit::a(h64 &a, h32 k);

void durchgangeinheit::operator()(void) {
	unique_lock<mutex> l(this->m);
	for (;;) {
		this->cv.wait(l, [this]{ return !this->ss || this->ube != einheit::nube; });
		if (!this->ss)
			break;
		h32 gf(unterbrechung::g(this->ube)), ka(unterbrechung::z(this->ube));
		this->ube = einheit::nube;
		l.unlock();
		this->se.l(this->uez, ka, gf);
		this->se.l(this->utz, ka + 4, gf);
		this->se.l(this->gfb, ka + 8, gf);
		this->se.l(this->az, ka + 12, gf);

		try {
			do {
				this->af();
			} while (this->ss && this->zs && this->ube == einheit::nube);
			if (!this->ss)
				break;
		} catch (sonderfalle const &e) {
			h64 ue;
			this->l(ue, this->uez);
			this->s(this->utz, ue);
			this->zs = false;
		}
		l.lock();
	}
}

bool durchgangeinheit::ls(void) {
	return this->zs;
}

void durchgangeinheit::af(void) {
	h64 a;
	this->a(a, this->az);

	h64 p(1ULL << (64 - 11));

	// Regelungsanweisung
	if (!((a >> (64 - 3)) & 1) && !((a >> (64 - 10)) & ((1U << 6) - 1))) {
		this->zs = !((a >> (64 - 1)) & 1);
		this->az += (vzw(a, p) >> 32) + 4;
	// Übertragungsanweisung
	} else if (((a >> (64 - 10)) & ((1U << 6) - 1)) < 3) {
		h32 z, ab;
		if ((a >> (64 - 4)) & 1) {
			// Beständergestalt
			z  = ((a & (p - 1)) >> 32);
			ab = (a & ((1ULL << 32) - 1));
			this->az += 8;
		} else {
			// Speichergestalt
			h32 ba((a & (p - 1)) >> 32);
			this->l(ab,  ba);
			this->l(z, ba + 4);
			this->az += 4;
		}

		if (!((a >> (64 - 10)) & ((1U << 6) - 1))) {
			// Lesung
			this->g.l(this, z, ab);
		} else {
			// Schreibung
			this->g.s(this, z, ab);
		}
		this->zs = !((a >> (64 - 1)) & 1);
	} else {
		this->az += this->g(this, a);
		this->zs  = !((a >> (64 - 1)) & 1);
	}
}
