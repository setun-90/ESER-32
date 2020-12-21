#include "../src/wahrspeicher.h"
#include "../src/verwandlungseinheit.h"
#include <sonderfalle.h>
#include <trace.h>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <bitset>

using namespace kunstspeicher;

int main(void) {
	std::ios_base::sync_with_stdio(false);

	TRACE("*** Hauptspeicheranfang");
	wahrspeicher hs(2048 + 5);
	TRACE("Hauptspeichergröße: " + std::to_string(hs.g()));

	TRACE("*** Beispielzugriffe in den Hauptspeicher");
	h64 q64(0x0000030405060708);
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
		throw std::logic_error(std::to_string(z64) + " != " + std::to_string(q64));

	TRACE("*** Beispielzugriffe in den Kunstspeicher");
	h32  gf(0x00804000),
		s2b(0x00200000),
		s2s(0x00802001), s2t(0x00600000);
	h32 s1f(0x00803800),
		s1b(0x00800800),
		s1s(0x00802011), s1t(0x00801800);

	verwandlungseinheit e(hs);

	hs.s(gf,                 s2b | ss::s);
	hs.s(gf + 8,             s2b);
	hs.s(gf + 12,            s2s);
	hs.s(gf + 16,            s1f);
	hs.s(s1::z & s1f,        s1b | ss::s);
	hs.s((s1::z & s1f) + 8,  s1b);
	hs.s((s1::z & s1f) + 12, s1s);
	hs.s(ss::z & s2s,        s2t);
	hs.s(ss::z & s1s,        s1t);
	for (auto ka: {(h32)0x003FFFFC, (h32)0x01000FFC}) {
		try {
			e.s(ka, gf, q64);
			throw std::logic_error("Unmapped address accepted");
		} catch (ZEE const &e) {
			TRACE(e.what());
		}
	}

	h32 ka1(0x00300FFC), ka2(0x010000FC), ka3(0x00BFFFFC), ka4(0x01002FFC);

	hs.s(gf,          s2b);
	hs.s(s1::z & s1f, s1b);
	e.enk(ka1,          gf);
	e.enk(ka2, s1::z & s1f);

	for (auto ez: {
		std::make_tuple(ka1, s2b,                gf),
		std::make_tuple(ka2, s1b,       s1f & s1::z),
		std::make_tuple(ka3, s2b,            gf + 8),
		std::make_tuple(ka4, s1b, (s1f & s1::z) + 8)
	}) {
		auto ka(std::get<0>(ez)), fe(std::get<1>(ez)), fa(std::get<2>(ez));
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.s(ka, gf, q64);
			throw std::logic_error("Forbidden write succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa, fe | ss::s);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.l(z64, ka, gf);
			throw std::logic_error("Forbidden read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa, fe | ss::l);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.a(z64, ka, gf);
			throw std::logic_error("Forbidden instruction read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa, fe | ss::a);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.g(z64, ka, gf);
			throw std::logic_error("Forbidden context entry read succeeded");
		} catch (ZZE const &e) {
			TRACE(e.what());
		}
		hs.s(fa, fe | ss::s);
	}

	for (auto ez: {
		std::make_tuple(ka1,                 gf,          gf),
		std::make_tuple(ka2,        s1::z & s1f, s1::z & s1f),
		std::make_tuple(ka3,            gf + 12, ss::z & s2s),
		std::make_tuple(ka4, (s1::z & s1f) + 12, ss::z & s1s)
	}) {
		auto ka(std::get<0>(ez)),
			sa(std::get<1>(ez)),
			ta(std::get<2>(ez));
		h32 fs, ft;
		hs.l(fs, sa);
		hs.s(sa, fs | ss::s);
		hs.l(ft, ta);
		hs.s(ta, ft | st::w);

		e.enk(ka, gf);
		e.enk(ka + 4, gf);
		try {
			e.s(ka, gf, q64);
			throw std::logic_error("Write to absent page succeeded");
		} catch (ZSW const &e) {
			TRACE(e.what());
		}
		hs.s(ta, ft & ~st::w);
		hs.s(sa, fs | ss::s | ss::l);
		e.enk(ka, gf);
		e.enk(ka + 4, gf);
	}

	hs.s(gf,                 s2b | ss::s | ss::l);
	hs.s(gf + 8,             s2b | ss::s | ss::l);
	hs.s(gf + 12,            s2s | ss::s | ss::l);
	hs.s(s1::z & s1f,        s1b | ss::s | ss::l);
	hs.s((s1::z & s1f) + 8,  s1b | ss::s | ss::l);
	hs.s((s1::z & s1f) + 12, s1s | ss::s | ss::l);
	for (auto ka: {
		ka1,
		ka2,
		ka3,
		ka4
	}) {
		TRACE(static_cast<std::ostringstream &>(std::ostringstream() << std::hex << std::setfill('0') << std::setw(8) << gf).str());
		e.s(ka, gf, q64);
		TRACE("");
		for (size_t i(0); i < sizeof q64; i++) {
			e.l(z8[i], ka + i, gf);
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
			throw std::logic_error(std::to_string(z64) + " != " + std::to_string(q64));
	}

	return 0;
}
