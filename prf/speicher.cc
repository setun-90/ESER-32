#include "../src/wahrspeicher.h"
#include "../src/verwandlungseinheit.h"
#include <sonderfalle.h>
#include <trace.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <bitset>

using namespace std;
using namespace kunstspeicher;

int main(void) {
	ios_base::sync_with_stdio(false);

	TRACE("*** Hauptspeicheranfang");
	wahrspeicher hs(2048 + 5);
	TRACE((ostringstream() << "Hauptspeichergröße: " << hs.g()).str().c_str());

	TRACE("*** Beispielzugriffe in den Hauptspeicher");
	h64 q64(0x0102030005060708_64);
	hs.s(7, q64);

	h8 z8[sizeof q64];
	for (size_t i(0); i < sizeof q64; i++) {
		hs.l(z8[i], 7 + i);
	};
	h64 z64((static_cast<h64>(z8[0]) << 56)
	  | (static_cast<h64>(z8[1]) << 48)
	  | (static_cast<h64>(z8[2]) << 40)
	  | (static_cast<h64>(z8[3]) << 32)
	  | (static_cast<h64>(z8[4]) << 24)
	  | (static_cast<h64>(z8[5]) << 16)
	  | (static_cast<h64>(z8[6]) << 8)
	  | static_cast<h64>(z8[7]));
	if (z64 != q64)
		throw logic_error((ostringstream() << z64 << " != " << q64).str().c_str());

	TRACE("*** Beispielzugriffe in den Kunstspeicher");
	h32  gf(0x00804000_32),
		s2b(0x00200000_32),
		s2s(0x00802001_32), s2t(0x00600000_32);
	h32 s1f(0x00803800_32),
		s1b(0x00800800_32),
		s1s(0x00802011_32), s1t(0x00801800_32);

	verwandlungseinheit e(hs);
	e.gf = gf;

	hs.s(gf,                    s2b | ss::s);
	hs.s(gf + 8_32,             s2b);
	hs.s(gf + 12_32,            s2s);
	hs.s(gf + 16_32,            s1f);
	hs.s(s1::z & s1f,           s1b | ss::s);
	hs.s((s1::z & s1f) + 8_32,  s1b);
	hs.s((s1::z & s1f) + 12_32, s1s);
	hs.s(ss::z & s2s,           s2t);
	hs.s(ss::z & s1s,           s1t);
	for (auto ka: {static_cast<h32>(0x003FFFFC_32), static_cast<h32>(0x01000FFC_32)}) {
		try {
			e.s(ka, q64);
			throw logic_error("Unmapped address accepted");
		} catch (ZEE const &e) {
			TRACE(e.what());
		}
	}

	h32 ka1(0x00300FFC_32), ka2(0x010000FC_32), ka3(0x00BFFFFC_32), ka4(0x01002FFC_32);

	hs.s(gf,          s2b);
	hs.s(s1::z & s1f, s1b);
	e.enk(ka1,          gf);
	e.enk(ka2, s1::z & s1f);

	for (auto ez: {
		make_tuple(ka1, s2b,                   gf, s2b,                    gf),
		make_tuple(ka2, s1b,          s1f & s1::z, s1b,           s1f & s1::z),
		make_tuple(ka3, s2b,            gf + 8_32, s2s,            gf + 12_32),
		make_tuple(ka4, s1b, (s1f & s1::z) + 8_32, s1s, (s1f & s1::z) + 12_32)
	}) {
		auto ka(get<0>(ez)), fe1(get<1>(ez)), fa1(get<2>(ez)), fe2(get<3>(ez)), fa2(get<4>(ez));
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		e.g(z64, ka);
		try {
			e.s(ka, q64);
			throw logic_error("Forbidden write succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa1, fe1 | ss::s);
		hs.s(fa2, fe2 | ss::s);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		e.s(ka, q64);
		try {
			e.l(z64, ka);
			throw logic_error("Forbidden read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa1, fe1 | ss::l);
		hs.s(fa2, fe2 | ss::l);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		e.l(z64, ka);
		try {
			e.a(z64, ka);
			throw logic_error("Forbidden instruction read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa1, fe1 | ss::a);
		hs.s(fa2, fe2 | ss::a);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		e.a(z64, ka);
		try {
			e.g(z64, ka);
			throw logic_error("Forbidden context entry read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa1, fe1 | ss::s);
		hs.s(fa2, fe2 | ss::s);
	}

	for (auto ez: {
		make_tuple(ka1,                   gf,          gf),
		make_tuple(ka2,          s1::z & s1f, s1::z & s1f),
		make_tuple(ka3,            gf + 8_32, ss::z & s2s),
		make_tuple(ka4, (s1::z & s1f) + 8_32, ss::z & s1s)
	}) {
		auto ka(get<0>(ez)),
			sa(get<1>(ez)),
			ta(get<2>(ez));
		h32 fs, ft;
		hs.l(fs, sa);
		hs.s(sa, fs | ss::s);
		hs.l(ft, ta);
		hs.s(ta, ft | st::w);

		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.s(ka, q64);
			throw logic_error("Write to absent page succeeded");
		} catch (ZSW const &e) {
			TRACE(e.what());
		}
		hs.s(ta, ft & ~st::w);
		hs.s(sa, fs | ss::s | ss::l);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
	}

	hs.s(gf,                    s2b | ss::s | ss::l);
	hs.s(gf + 8_32,             s2b | ss::s | ss::l);
	hs.s(gf + 12_32,            s2s | ss::s | ss::l);
	hs.s(s1::z & s1f,           s1b | ss::s | ss::l);
	hs.s((s1::z & s1f) + 8_32,  s1b | ss::s | ss::l);
	hs.s((s1::z & s1f) + 12_32, s1s | ss::s | ss::l);
	for (auto ka: {
		ka1,
		ka2,
		ka3,
		ka4
	}) {
		TRACE((ostringstream() << hex << setfill('0') << setw(8) << gf).str().c_str());
		e.s(ka, q64);
		TRACE("");
		for (size_t i(0); i < sizeof q64; i++) {
			e.l(z8[i], ka + i);
		};
		TRACE("");
		z64 = (static_cast<h64>(z8[0]) << 56)
		    | (static_cast<h64>(z8[1]) << 48)
		    | (static_cast<h64>(z8[2]) << 40)
		    | (static_cast<h64>(z8[3]) << 32)
		    | (static_cast<h64>(z8[4]) << 24)
		    | (static_cast<h64>(z8[5]) << 16)
		    | (static_cast<h64>(z8[6]) << 8)
		    | static_cast<h64>(z8[7]);
		if (z64 != q64)
			throw logic_error((ostringstream() << z64 << " != " << q64).str().c_str());
	}

	return 0;
}
