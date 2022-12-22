#include "durchgangeinheit.h"

#include <sonderfalle.h>
#include <unterbrechung.h>

using namespace std;
using namespace kunstspeicher;



durchgangeinheit::durchgangeinheit(wahrspeicher &hs):
	einheit(hs), zs(false) {}

void durchgangeinheit::operator()(void) {
	unique_lock<mutex> l(this->m);
	for (;;) {
		this->cv.wait(l, [this]{ return !this->ss || this->ube != einheit::nube; });
		if (!this->ss)
			break;
		h32 gf(unterbrechung::g(this->ube)), ka(unterbrechung::z(this->ube));
		this->ube = einheit::nube;
		l.unlock();
		this->se.gf = gf;
		this->se.l(this->uez, ka);
		this->se.l(this->utz, ka + 4);
		this->se.l(this->gfb, ka + 8);
		this->se.l(this->az, ka + 12);

		try {
			do {
				this->af();
			} while (this->ss && this->zs && this->ube == einheit::nube);
			if (!this->ss)
				break;
		} catch (sonderfalle const &e) {
			h64 ue;
			this->se.l(ue, this->uez);
			this->se.s(this->utz, ue);
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
	this->se.a(a, this->az);

	this->zs = !stelle<1>(a);
	// Regelungsanweisung
	if (!stelle<3>(a) && !feld<5, 10>(a)) {
		h32 ba(vzw<ss1<h32>(11)>(feld<11, 32>(a)) + 4);
		if (stelle<4>(a)) {
			// Beständergestalt
			this->az += ba;
		} else {
			// Speichergestalt
			h32 ab;
			this->se.l(ab, this->az + ba);
			this->az = ab;
		}
	// Übertragungsanweisung
	} else if (feld<5, 10>(a) < 3) {
		h32 z, ab;
		if (stelle<4>(a)) {
			// Beständergestalt
			z  = feld<11, 32>(a);
			ab = feld<33, 64>(a) + this->az;
			this->az += 8;
		} else {
			// Speichergestalt
			h32 ba(feld<11, 32>(a) + this->az);
			this->se.l(ab,  ba);
			this->se.l(z, ba + 4);
			this->az += 4;
		}

		if (!feld<5, 10>(a)) {
			// Lesung
			this->l(z, ab);
		} else {
			// Schreibung
			this->s(z, ab);
		}
	} else {
		this->az += (*this)(a);
	}
}
