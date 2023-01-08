#include "recheneinheit.h"

#include <sonderfalle.h>
#include <unterbrechung.h>

using namespace std;
using namespace kunstspeicher;



recheneinheit::recheneinheit(wahrspeicher &hs):
	einheit(hs), b(0), zs(false), zes(false) {
	this->ns[0] = 0;
}
bool recheneinheit::ls(void) {
	return this->zs;
}

void recheneinheit::operator()(void) {
	unique_lock<mutex> l(this->m);
	for (;;) {
		this->cv.wait(l, [this]{ return !this->ss || this->ube != einheit::nube; });
		if (!this->ss)
			break;
		h32 gf(unterbrechung::g(this->ube)), ka(unterbrechung::z(this->ube));
		this->ube = einheit::nube;
		l.unlock();

		h64 ze;
		this->se.gf = gf;
		this->se.g(ze, ka);
		h32 ngfb((ze >> 32) & 0xFFFFF000_32);
		if (this->zes) {
			h64 aze((static_cast<h64>(this->gfb | (this->b << 1)) << 32) | this->az | this->zs);
			this->se.gf = ngfb - 16;
			this->se.s(0, aze);
		}
		this->se.gf = this->gfb = ngfb;
		this->b     = ((ze >> 32) & 0x00000006_32) >> 1;
		this->az    = ze & 0xFFFFFFFE_32;
		this->zs    = static_cast<bool>(ze & 1);

		if (!this->zs)
			break;
		this->zes =  true;
		try {
			do {
				this->af();
			} while (this->ss && this->zs && this->ube == einheit::nube);
		} catch (sonderfalle const &s) {
			// Gastfehler
			//host.trace(s.what());
			this->zs = false;
		} catch (exception const &e) {
			// Host error
			this->sf = current_exception();
			this->ss = this->zs = false;
		}
		if (!this->ss)
			break;
		l.lock();
	}
	// TODO: Notify host of error
	if (this->sf);
}

void recheneinheit::nss(h8 z, h32 a) {
	if (z) this->ns[z] = a;
}
void recheneinheit::nsl(h32 &a, h8 q) {
	a = q ? this->ns[q] : 0;
}
h8 recheneinheit::zb(h32 a) {
	return !a ? 0 : a > 0 ? 1 : 2;
}

void recheneinheit::af(void) {
	h32 a;
	this->se.a(a, this->az);
	// Regelungsanweisungen
	if (!stelle<3>(a)) {
		// Alle Verweiterungen dieser Abteilung des Entwurfs hier.

		// Gestalten
		h32 q(0);
		switch ((a >> a_g) & 0x3) {
		case 0x0: { // Beständergestalt
			this->az += 4;
			q = vzw(a & a_w, 32 - 13);
			break;
		}
		case 0x3: { // Nahspeichergestalt
			this->az += 2;
			this->nsl(q, (a >> a_z) & 0xF);
			break;
		}
		case 0x1: { // Hauptspeichergestalt
			this->az += 4;
			h32 t;
			this->nsl(t, (a >> a_z) & 0xF);
			h32 qa(t + (a & a_a));
			this->se.l(q, qa);
			break;
		}
		}
		h32 z((a >> 20) & 0xF);
		this->nss(z, this->az);
		if (a & (1 << (27 - this->b))) {
			if (!stelle<28>(a)) {
				// WGL - Weglass
				this->az += q;
			} else {
				// SPR - Sprung
				this->az = q;
			}
		}
		return;
	}
	// Bewegungsanweisungen
	if (stelle<3>(a)) {
		// Gestalten
		h32 q;
		switch ((a >> 24) & 0xF) {
		case 0x0: { // BWG - Bewegung
			switch ((a >> a_g) & 0x3) {
			case 0x0: { // Beständergestalt
				az += 4;
				h32 q(a & a_w);
				this->b = recheneinheit::zb(q);
				this->nss((a >> a_z) & 0xF, q);
				break;
			}
			case 0x3: { // Nahspeichergestalt
				az += 2;
				this->nsl(q, (a >> a_q) & 0xF);
				this->b = recheneinheit::zb(q);
				this->nss((a >> a_z) & 0xF, q);
				break;
			}
			case 0x1: { // Hauptspeicherquellegestalt
				az += 4;
				h32 g, gns((a >> a_q) & 0xF), zns((a >> a_z) & 0xF);
				this->nsl(g, gns);
				if (a & (1 << 17)) {
					this->se.l(q, g);
					this->b = recheneinheit::zb(q);
					this->nss(zns, q);
					this->nss(gns, g + (a & a_a));
				} else {
					this->se.l(q, g + (a & a_a));
					this->b = recheneinheit::zb(q);
					this->nss(zns, q);
				}
				break;
			}
			case 0x2: { // Hauptspeicherzielgestalt
				az += 4;
				h32 g, gns((a >> a_q) & 0xF), qns((a >> a_z) & 0xF);
				this->nsl(g, gns);
				if (a & (1 << 17)) {
					g += (a & a_a);
					this->nss(gns, g);
					this->nsl(q, qns);
					this->b = recheneinheit::zb(q);
					this->se.s(g, q);
				} else {
					this->nsl(q, qns);
					this->b = recheneinheit::zb(q);
					this->se.s(g + (a & a_a), q);
				}
				break;
			}
			}
			break;
		}
		case 0x1: { // UWG - Ungeschnittende Bewegung
			switch ((a >> a_g) & 0x3) {
			case 0x1: { // Hauptspeicherquellegestalt
				az += 4;
				h8 z((a >> a_z) & 0xF);
				break;
			}
			case 0x2: { // Hauptspeicherzielgestalt
				az += 4;
				break;
			}
			}
			break;
		}
		case 0x2: { // Zustandseintragsanweisungen
			switch ((a >> a_g) & 0x3) {
			case 0x1: { // VWE - Verfahrenwechsel (Ladung)
				h32 q;
				{
					h32 t;
					this->nsl(t, (a >> a_q) & 0xF);
					q = t + (a & a_ra);
				}
				h64 ze;
				this->se.g(ze, q);
				this->ube = ze;
				this->zes = false;
				break;
			}
			case 0x2: { // UTB - Unterbrechung
				h64 q;
				{
					h32 t1, t2;
					this->nsl(t1, (a >> a_q) & 0xE);
					this->nsl(t2, ((a >> a_q) & 0xE) + 1);
					q = (((h64)t1) << 32) | t2;
				}
				h32 ut;
				{
					h32 t;
					this->nsl(t, (a >> a_z) & 0xE);
					ut = t + (a & a_ra);
				}
				this->se.s(ut, q);
				break;
			}
			}
			break;
		}
		default:
			throw AUA(this->az, this->gfb, a);
		}
		return;
	}
	// Rechnungsanweisungen
	{
		// Gestalten
		bool nsz(false);
		h32 z, q;
		h8 g((a >> a_g) & 0x3);
		switch (g) {
		case 0x0: { // Beständergestalt
			az += 4;
			nsz = true;
			z = (a >> a_z) & 0xF;
			q = vzw(a & a_w, 20);
			break;
		}
		case 0x3: { // Nahspeichergestalt
			az += 2;
			nsz = true;
			z = (a >> a_z) & 0xF;
			this->nsl(q, (a >> a_q) & 0xF);
			break;
		}
		case 0x1: { // Hauptspeicherquellegestalt
			az += 4;
			nsz = true;
			z = (a >> a_z) & 0xF;
			h32 gr;
			this->nsl(gr, (a >> a_z) & 0xF);
			if (a & (1 << a_an)) {
				this->se.l(q, gr);
			} else {
				this->se.l(q, gr + vzw(a & a_a, 12));
			}
			break;
		}
		case 0x2: { // Hauptspeicherzielgestalt
			az += 4;
			h32 zns((a >> a_z) & 0xF);
			this->nsl(z, zns);
			if (a & (1 << a_an)) {
				z += vzw(a & a_a, 12);
				this->nss(zns, z);
			}
			break;
		}
		}
		h32 qz;
		if (nsz) {
			this->nsl(qz, z);
		} else {
			this->se.l(qz, z);
		}
		switch ((a >> 28) & 0xF) {
		case 0x0: { // LVS - Linksverschiebung
			q %= 32;
			if (q) qz <<= q;
			break;
		}
		case 0x1: { // RNS - Rechtsnullverschiebung
			q %= 32;
			if (q) qz >>= q;
			break;
		}
		case 0x2: { // RZS - Rechtsvorzeichenverschiebung
			q %= 32;
			if (q) qz = vzw(qz >> q, 32 - q);
			break;
		}
		case 0x3: { // DRH - Umdreh
			q %= 32;
			qz = q ? (qz >> q) | (qz << (32 - q)) : qz;
			break;
		}
		case 0x4: { // UND - Durchschnitt
			qz &= q;
			break;
		}
		case 0x5: { // ODR - Vereinigung
			qz |= q;
			break;
		}
		case 0x6: { // NIC - Nicht
			qz = ~qz;
			break;
		}
		case 0x7: { // VZW - Vorzeichenwechsel
			qz = ~qz + 1;
			break;
		}
		case 0x8: { // ZGB - Zugabe
			qz += q;
			break;
		}
		case 0x9: { // UNT - Unterschied
			qz -= q;
			break;
		}
		case 0xA: { // VFL - Verfaltung
			qz *= q;
			break;
		}
		case 0xB: { // VTL - Verteilung
			qz /= q;
			break;
		}
		}
		switch (g) {
		case 0x1: {
			if (a & (1 << a_an)) {
				this->nss((a >> a_q) & 0xF, g + (a & a_a));
			}
			break;
		}
		case 0x2: {
			break;
		}
		}
	}

	throw AUA(this->az, this->gfb, a);
}
