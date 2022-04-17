#include <sonderfalle.h>
#include "recheneinheit.h"

using namespace std;
using namespace kunstspeicher;



recheneinheit::recheneinheit(wahrspeicher &hs):
	einheit(hs), zs(false) {}

void recheneinheit::operator()(void) {
	do {
#if __cplusplus >= 202002L
		while (this->ube == einheit::nube)
			this->ube.wait(einheit::nube);
		h64 aze(0);
		if (this->zs) {
			aze = ((h64)(this->gfb | (this->b << 1)) << 32) | this->az | this->zs;
		}
		this->gfb = (this->ube >> 32) & 0xFFFFF000;
		this->b   = (this->ube >> 32) & 0x00000006;
		this->az  =  this->ube & 0xFFFFFFFE;
		this->zs  =  this->ube & 1;
		if (aze & 1) {
			this->s(0, aze);
		}
		this->ube =  einheit::nube;
		if (this->zs) {
			h32 a;
			do {
				this->a(a, this->az);
				this->af(a);
			} while (this->an && this->ube == einheit::nube);
		}
#else
		{
			unique_lock<mutex> l(this->m);
			this->cv.wait(l, [this]{ return !this->an || this->ube != einheit::nube; });
			if (!this->an)
				break;
			h64 aze(0);
			if (this->zs) {
				aze = ((h64)(this->gfb | (this->b << 1)) << 32) | this->az | this->zs;
			}
			this->gfb = (this->ube >> 32) & 0xFFFFF000;
			this->b   = (this->ube >> 32) & 0x00000006;
			this->az  =  this->ube & 0xFFFFFFFE;
			this->zs  =  bool(this->ube & 1);
			if (aze & 1) {
				this->s(0, aze);
			}
			this->ube =  einheit::nube;
		}
		if (this->an && this->zs) {
			h32 a;
			do {
				this->a(a, this->az);
				this->af(a);
			} while (this->an && this->ube == einheit::nube);
		}
#endif
	} while (this->an);
}

bool recheneinheit::ls(void) {
	return this->zs;
}

void recheneinheit::nss(h8 z, h32 a) {
	if (z) this->ns[z] = a;
}
void recheneinheit::nsl(h32 &a, h8 q) {
	a = q ? this->ns[q] : 0;
}

void recheneinheit::af(h32 a) {
	// Regelungsanweisungen
	if (!(a & (1 << (a_g - 1)))) {
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
			this->l(q, qa);
			break;
		}
		}
		h32 z((a >> 20) & 0xF);
		this->nss(z, this->az);
		if (a & (1 << (27 - this->b))) {
			if (!(a & (1 << 28))) {
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
	if (a & (1 << 29)) {
		// Gestalten
		h32 q;
		switch ((a >> 24) & 0xF) {
		case 0x0: { // BWG - Bewegung
			switch ((a >> a_g) & 0x3) {
			case 0x0: { // Beständergestalt
				az += 4;
				this->nss((a >> a_z) & 0xF, (a & a_w));
				break;
			}
			case 0x3: { // Nahspeichergestalt
				az += 2;
				this->nsl(q, (a >> a_q) & 0xF);
				this->nss((a >> a_z) & 0xF, q);
				break;
			}
			case 0x1: { // Hauptspeicherquellegestalt
				az += 4;
				h32 g, gns((a >> a_q) & 0xF), zns((a >> a_z) & 0xF);
				this->nsl(g, gns);
				if (a & (1 << 17)) {
					this->l(q, g);
					this->nss(zns, q);
					this->nss(gns, g + (a & a_a));
				} else {
					this->l(q, g + (a & a_a));
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
					this->s(g, q);
				} else {
					this->nsl(q, qns);
					this->s(g + (a & a_a), q);
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
			h32 q;
			{
				h32 t;
				this->nsl(t, (a >> a_q) & 0xF);
				q = t + (a & a_ra);
			}
			h64 ze;
			this->se.g(ze, q, this->gfb);
			this->ube = ze;
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
				this->l(q, gr);
			} else {
				this->l(q, gr + vzw(a & a_a, 12));
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
			this->l(qz, z);
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
