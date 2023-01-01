#include "../src/verbindung.h"
#include <trace.h>

#include <thread>
#include <chrono>

using namespace std;
using namespace kunstspeicher;

int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	if (argc <= 1) {
		cerr << string("Usage: ").append(argv[0]).append(" $Prufung_Device_Plugin\n");
		return 1;
	}

	/* Main memory */
	wahrspeicher hs(2048 + 5);
	h32  gf(0x00804000_32),
		s2b(0x00200000_32),
		s2s(0x00802001_32), s2t(0x00600000_32);
	h32 s1f(0x00803800_32),
		s1b(0x00800800_32),
		s1s(0x00802011_32), s1t(0x00801800_32);

	hs.s(gf,                    s2b | ss::a);
	hs.s(gf + 8_32,             s2b | ss::l);
	hs.s(gf + 12_32,            s2s | ss::s | ss::l);
	hs.s(gf + 16_32,            s1f);
	hs.s(s1::z & s1f,           s1b | ss::s);
	hs.s((s1::z & s1f) + 8_32,  s1b);
	hs.s((s1::z & s1f) + 12_32, s1s);
	hs.s(ss::z & s2s,           s2t);
	hs.s(ss::z & s1s,           s1t);

	/* Channel interrupt data structure */
	h32 aaf(0x00000200_32);
	hs.s(0_32,  static_cast<h32>(0x00800000_32));
	hs.s(4_32,  static_cast<h32>(0x00C00010_32));
	hs.s(8_32,  gf);
	hs.s(12_32, aaf);

	/* Channel program */
	h32 az(aaf);
	/**** Jump instructions */
	hs.s(az, static_cast<h32>(0x10000000_32)); az += 4;
	hs.s(az, static_cast<h32>(0x10000008_32)); az += 4;
	hs.s(az, static_cast<h32>(0x10000008_32)); az += 4;
	hs.s(az, static_cast<h32>(0x10000000_32)); az += 4;
	hs.s(az, static_cast<h32>(0x103FFFF4_32)); az += 4;

	/**** Transfer instructions */
	hs.s(az, static_cast<h64>(0xB000000000000000_64 | (static_cast<h64>(0x00000800_32 - az) << 32) | 0x000003FF_32)); az += 8;
	hs.s(az, static_cast<h64>(0xB040000000000000_64 | (static_cast<h64>(0x00000800_32 - az) << 32) | 0x000003FF_32)); az += 8;

	istringstream i;
	shared_ptr<einheit> e(durchgangeinheit::verbindung(argv[1]).abb(hs, i));
	e->an();
	e->ub(static_cast<h64>(0x0080400000800000_64));
	this_thread::sleep_for(chrono::milliseconds(1000));
	e->ab();

	return 0;
}
