#include <sonderfalle.h>
#include <unterbrechung.h>
#include "durchgangeinheit.h"
#include <utility>

using namespace std;
using namespace kunstspeicher;



durchgangeinheit::durchgangeinheit(wahrspeicher &e, gerat &&g):
	einheit(e), zs(false), gr(move(g)) {}

void durchgangeinheit::ubv(void) {
	h32 gf(unterbrechung::g(this->ube)), ka(unterbrechung::z(this->ube));
	this->se.l(this->uez, ka, gf);
	this->se.l(this->utz, ka + 4, gf);
	this->se.l(this->gfb, ka + 8, gf);
	this->se.l(this->az, ka + 12, gf);
}
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
#if __cplusplus >= 202002L
	do {
		while (this->ube == einheit::nube)
			this->ube.wait(einheit::nube);
		this->ubv();
		this->ube = einheit::nube;
		this->zs = true;
		h64 a;
		try {
			do {
				this->a(a, this->az);
				this->af(a);
			} while (this->zs && this->ube == einheit::nube);
		} catch (sonderfalle const &e) {
			h64 ue;
			this->l(ue, this->uez);
			this->s(this->utz, ue);
			this->zs = false;
		}
	} while (this->an);
#else
	unique_lock<mutex> l(this->m);
	while (this->an) {
		this->ubv();
		this->ube = einheit::nube;
		l.unlock();

		this->zs = true;
		try {
			h64 a;
			do {
				this->a(a, this->az);
				this->af(a);
			} while (this->an && this->zs && this->ube == einheit::nube);
		} catch (sonderfalle const &e) {
			h64 ue;
			this->l(ue, this->uez);
			this->s(this->utz, ue);
			this->zs = false;
		}
		l.lock();
		this->cv.wait(l, [this]{ return !this->an || this->ube != einheit::nube; });
	}
#endif
}

bool durchgangeinheit::ls(void) {
	return this->zs;
}

void durchgangeinheit::af(h64 a) {
	h64 p(1ULL << (64 - 11));

	if ((a >> (64 - 9)) & ((1 << 6) - 1)) {
		this->gr(this, a);
		return;
	}
	// Regelungsanweisung
	if (!(a & (1ULL << (64 - 3)))) {
		switch ((a & (p - 1)) >> 32) {
		case 0: {
			az += (vzw(a, p) >> 32) + 4;
			break;
		}
		// Verweiterungen dieser Teil des Entwurfs hier.
		}
		return;
	// Übertragungsanweisung
	} else {
		h32 z, ab;
		if (a & (1ULL << (64 - 5))) {
			// Beständergestalt
			z  = ((a & (p - 1)) >> 32);
			ab = (a & ((1ULL << 32) - 1));
		} else {
			// Speichergestalt
			h32 ba((a & (p - 1)) >> 32);
			this->l(ab,  ba);
			this->l(z, ba + 4);
		}

		if (a & (1ULL << (64 - 4))) {
			// Lesung
			this->gr.l(this, z, ab);
		} else {
			// Schreibung
			this->gr.s(this, z, ab);
		}
		return;
	}

	throw AUA(this->az, this->gfb, a);
}
