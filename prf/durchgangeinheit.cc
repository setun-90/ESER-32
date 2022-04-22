#include "../src/durchgangeinheit.h"
#include <trace.h>
#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include <memory>

using namespace std;
using namespace kunstspeicher;

int main(void) {
	ios_base::sync_with_stdio(false);

	/* Main memory */
	wahrspeicher hs(2048 + 5);
	h32  gf(0x00804000),
		s2b(0x00200000),
		s2s(0x00802001), s2t(0x00600000);
	h32 s1f(0x00803800),
		s1b(0x00800800),
		s1s(0x00802011), s1t(0x00801800);

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
	h32 aaf(0x00000200);
	hs.s(0,  (h32)0x00800000);
	hs.s(4,  (h32)0x00000010);
	hs.s(8,  (h32)gf);
	hs.s(12, (h32)aaf);

	/* Channel program */
	hs.s(aaf,      (h32)0x00000000);
	hs.s(aaf + 4,  (h32)0x00000008);
	hs.s(aaf + 8,  (h32)0x10000000);
	hs.s(aaf + 12, (h32)0x10000000);
	hs.s(aaf + 16, (h32)0x003FFFF8);


	auto m(dlopen("./lib/prufung.so", RTLD_LAZY));
	char *f;
	if (f = dlerror()) {
		TRACE("Ladung ist gescheitert: " + string(f));
		return 1;
	}
	durchgangeinheit::gerat &&(*abb)(istringstream &)
		(reinterpret_cast<durchgangeinheit::gerat &&(*)(istringstream &)>(dlsym(m, "abb")));
	if (f = dlerror()) {
		TRACE("Anschalt ist gescheitert: " + string(f));
		return 1;
	}
	istringstream i;
	shared_ptr<einheit> e(make_shared<durchgangeinheit>(hs, abb(i)));
	e->ub((h64)0x0080400000800000);
	while (!e->ls());
	while (e->ls());
	e->as();

	return 0;
}
