#include "../src/recheneinheit.h"
#include <memory>
#include <ios>
#include <trace.h>

using namespace std;
using namespace kunstspeicher;

int main(void) {
	ios_base::sync_with_stdio(false);

	/* Main memory */
	wahrspeicher hs(10);
	h32  gf1(0x00000000U), gf2(0x00001000U);
	h32  s1f1(0x00002800U), s1f2(0x00003800U);
	h32  sgs(0x00000FF1U), sgt(0x00004000U), sb1(0x00005800U), sb2(0x00006800U), sba(0x00007800U), sbe(0x00009800U);

	hs.s(gf1, s1f1);
	hs.s(gf2, s1f2);
	hs.s(s1::z & s1f1,        sgs | ss::s);
	hs.s((s1::z & s1f1) + 8,  sb1 | ss::s | ss::l);
	hs.s((s1::z & s1f1) + 12, sba | ss::a);
	hs.s((s1::z & s1f1) + 16, sbe);
	hs.s(s1::z & s1f2,        sgs | ss::l);
	hs.s((s1::z & s1f2) + 8,  sb2 | ss::s | ss::l);
	hs.s((s1::z & s1f2) + 12, sba | ss::a);
	hs.s((s1::z & s1f2) + 16, sbe);

	h64 ze(0x3001ULL);
	hs.s(ss::z & sgs, sgt);
	hs.s((ss::z & sbe) + 8,  static_cast<h64>(0ULL));
	hs.s((ss::z & sbe) + 16, ze);

	/* Test program */
	h32 aaf(s1::z & sba), az(aaf);
	/**** Jump instructions */
	hs.s(az, static_cast<h32>(0x00000004U)); az += 4;
	hs.s(az, static_cast<h32>(0x0F000004U)); az += 4;
	hs.s(az, static_cast<h32>(0x0F000004U)); az += 4;
	hs.s(az, static_cast<h32>(0x0F0FFFF8U)); az += 4;
	hs.s(az, static_cast<h16>(0xCF10U)); az += 2;

	/**** Move and conditional jump instructions */
	hs.s(az, static_cast<h16>(0xF010U)); az += 2;
//	hs.s(az, 0x07000004); az += 4;
//	hs.s(az, 0x180F0000); az += 4;

	/**** Interrupt next unit */
//	hs.s(az, 0xB2004008); az += 4;

	/**** Shutdown */
	hs.s(az, static_cast<h32>(0x72004008U));


	shared_ptr<einheit> e1(make_shared<recheneinheit>(hs));
	e1->an();
	e1->ub(ze);
	while (!e1->ls());
	while (e1->ls());
	e1->ab();

	return 0;
}
