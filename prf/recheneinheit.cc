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
	h32  gf1(0x00000000_32), gf2(0x00001000_32);
	h32  s1f1(0x00002800_32), s1f2(0x00003800_32);
	h32  sgs(0x00000FF1_32), sgt(0x00004000_32), sb1(0x00005800_32), sb2(0x00006800_32), sba(0x00007800_32), sbe(0x00009800_32);

	hs.s(gf1, s1f1);
	hs.s(gf2, s1f2);
	hs.s(s1f1 & s1::z,           sgs | ss::s);
	hs.s((s1f1 & s1::z) + 8_32,  sb1 | ss::s | ss::l);
	hs.s((s1f1 & s1::z) + 12_32, sba | ss::a);
	hs.s((s1f1 & s1::z) + 16_32, sbe);
	hs.s(s1f2 & s1::z,           sgs | ss::l);
	hs.s((s1f2 & s1::z) + 8_32,  sb2 | ss::s | ss::l);
	hs.s((s1f2 & s1::z) + 12_32, sba | ss::a);
	hs.s((s1f2 & s1::z) + 16_32, sbe);

	h64 ube(0x4000_64), ze(0x3001_64);
	hs.s(sgs & ss::z, sgt);
	hs.s((sbe & ss::z) + 8_32,  0_64);
	hs.s((sbe & ss::z) + 16_32, ze);

	/* Test program */
	h32 aaf(sba & s1::z), az(aaf);
	/**** Jump instructions */
	hs.s(az, 0x00000004_32); az += 4;
	hs.s(az, 0x0F000004_32); az += 4;
	hs.s(az, 0x0F000004_32); az += 4;
	hs.s(az, 0x0F0FFFF8_32); az += 4;
	hs.s(az, 0xCF10_16); az += 2;

	/**** Move and conditional jump instructions */
	hs.s(az, 0xF010_16); az += 2;
//	hs.s(az, 0x07000004); az += 4;
//	hs.s(az, 0x180F0000); az += 4;

	/**** Interrupt next unit */
//	hs.s(az, 0xB2004008); az += 4;

	/**** Shutdown */
	hs.s(az, 0x72004008_32);


	shared_ptr<einheit> e1(make_shared<recheneinheit>(hs));
	e1->an();
	e1->ub(ube);
//	while (!e1->ls());
	while (e1->ls());
	e1->ab();

	return 0;
}
