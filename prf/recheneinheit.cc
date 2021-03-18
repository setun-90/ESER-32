#include "../src/recheneinheit.h"
#include <trace.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <set>

using namespace kunstspeicher;

int main(void) {
	std::ios_base::sync_with_stdio(false);

	/* Main memory */
	wahrspeicher hs(8);
	h32  gf1(0x00000000), gf2(0x00001000);
	h32  s1f1(0x00002800), s1f2(0x00003800);
	h32  sgb(0x00004011), sb1(0x00005800), sb2(0x00006800), sba(0x00007800);

	hs.s(gf1, s1f1);
	hs.s(gf2, s1f2);
	hs.s(s1::z & s1f1,        sgb | ss::s);
	hs.s((s1::z & s1f1) + 8,  sb1 | ss::s | ss::l);
	hs.s((s1::z & s1f1) + 12, sba | ss::a);
	hs.s(s1::z & s1f2,        sgb | ss::l);
	hs.s((s1::z & s1f2) + 8,  sb2 | ss::s | ss::l);
	hs.s((s1::z & s1f2) + 12, sba | ss::a);

	/* Test program */
	h32 aaf(0x00007000);
}
