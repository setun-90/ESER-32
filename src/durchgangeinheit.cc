#include <sonderfalle.h>
#include "durchgangeinheit.h"

using namespace std;
using namespace kunstspeicher;



durchgangeinheit::durchgangeinheit(wahrspeicher &e, unique_ptr<gerat> &&g):
	einheit(e), zs(false), gr(std::move(g)) {}

template <class art> void durchgangeinheit::s(h32 k, art a) {
	this->se.s(k, this->gfb, a);
}
template <class art> void durchgangeinheit::l(art &a, h32 k) {
	this->se.l(a, k, this->gfb);
}
template <class art> void durchgangeinheit::a(art &a, h32 k) {
	this->se.a(a, k, this->gfb);
}

void durchgangeinheit::ubv(void) {
	h32 gf(this->ube & feld<h64>(64 - 1, 64 - 20)), ka(this->ube & feld<h64>(64 - 33, 0));
	this->se.l(this->uez, ka, gf);
	this->se.l(this->utz, ka + 4, gf);
	this->se.l(this->gfb, ka + 8, gf);
	this->se.l(this->az, ka + 12, gf);
}

void durchgangeinheit::operator()(void) {
	for (;;) {
#if __cplusplus >= 202002L
		if (this->ube == einheit::nube)
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
#else
		{
			std::unique_lock<std::mutex> l(this->m);
			if (this->ube == einheit::nube)
				this->cv.wait(l, [this]{ return this->ube != einheit::nube; });
			this->ubv();
			this->ube = einheit::nube;
		}
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
#endif
	}
}

void durchgangeinheit::af(h64 a) {
	h64 p(1ULL << (64 - 11));

	if (a & feld<h64>(64 - 4, 64 - 10)) {
		this->gr->operator()(this, a);
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
			this->l(z,  ba);
			this->l(ab, ba + 4);
		}

		if (a & (1ULL << (64 - 4))) {
			// Lesung
			this->gr->l(this, z, ab);
		} else {
			// Schreibung
			this->gr->s(this, z, ab);
		}
		return;
	}

	throw AUA(this->az, this->gfb, a);
}
