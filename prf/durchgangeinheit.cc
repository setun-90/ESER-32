#include "../src/durchgangeinheit.h"
#include <trace.h>

using namespace std;
using namespace kunstspeicher;

int main(void) {
	ios_base::sync_with_stdio(false);

	/* Main memory */
	wahrspeicher hs(2048 + 5);
	h32  gf(0x00804000U),
		s2b(0x00200000U),
		s2s(0x00802001U), s2t(0x00600000U);
	h32 s1f(0x00803800U),
		s1b(0x00800800U),
		s1s(0x00802011U), s1t(0x00801800U);

	hs.s(gf,                 s2b | ss::a);
	hs.s(gf + 8,             s2b | ss::l);
	hs.s(gf + 12,            s2s | ss::s | ss::l);
	hs.s(gf + 16,            s1f);
	hs.s(s1::z & s1f,        s1b | ss::s);
	hs.s((s1::z & s1f) + 8,  s1b);
	hs.s((s1::z & s1f) + 12, s1s);
	hs.s(ss::z & s2s,        s2t);
	hs.s(ss::z & s1s,        s1t);

	/* Channel interrupt data structure */
	h32 aaf(0x00000200U);
	hs.s(0,  static_cast<h32>(0x00800000U));
	hs.s(4,  static_cast<h32>(0x00C00010U));
	hs.s(8,  gf);
	hs.s(12, aaf);

	/* Channel program */
	h32 az(aaf);
	/**** Jump instructions */
	hs.s(az, static_cast<h32>(0x10000000U)); az += 4;
	hs.s(az, static_cast<h32>(0x10000008U)); az += 4;
	hs.s(az, static_cast<h32>(0x10000008U)); az += 4;
	hs.s(az, static_cast<h32>(0x10000000U)); az += 4;
	hs.s(az, static_cast<h32>(0x103FFFF4U)); az += 4;

	/**** Transfer instructions */
	hs.s(az, static_cast<h64>(0xB000000000000000U | (static_cast<h64>(0x00000800U - az) << 32) | 0x000003FFU)); az += 8;
	hs.s(az, static_cast<h64>(0xB040000000000000U | (static_cast<h64>(0x00000800U - az) << 32) | 0x000003FFU)); az += 8;

	istringstream i;
	shared_ptr<einheit> e(make_shared<durchgangeinheit>(hs, vb("./debug/lib/prufung.so", i)));
	e->an();
	e->ub(static_cast<h64>(0x0080400000800000U));
	while (!e->ls());
	while (e->ls());
	e->ab();

	return 0;
}
