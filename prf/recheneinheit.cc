#include "../src/recheneinheit.h"
#include <memory>
#include <trace.h>

using namespace std;
using namespace kunstspeicher;

int main(void) {
	ios_base::sync_with_stdio(false);

	/* Main memory */
	wahrspeicher hs(10);
	h32  gf1(0x00000000), gf2(0x00001000);
	h32  s1f1(0x00002800), s1f2(0x00003800);
	h32  sgs(0x00000FF1), sgt(0x00004000), sb1(0x00005800), sb2(0x00006800), sba(0x00007800), sbe(0x00009800);

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
	hs.s((ss::z & sbe) + 8,  0ULL);
	hs.s((ss::z & sbe) + 16, ze);

	/* Test program */
	h32 aaf(s1::z & sba);
	/**** Jump instructions */
	hs.s(aaf,      0x00000004);
	hs.s(aaf + 4,  0x0F000004);
	hs.s(aaf + 8,  0x0F00000C);
	hs.s(aaf + 12, 0x0F0FFFF8);

	/**** Move instructions */
	hs.s(aaf + 16, 0x30102000);

	/**** Shutdown */
	hs.s(aaf + 24, 0x72004008);


	shared_ptr<einheit> e1(make_shared<recheneinheit>(hs));
	e1->ub(ze);
	while (e1->ls());
	e1->as();

	return 0;
}
