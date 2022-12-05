#include <sonderfalle.h>

#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;
using namespace kunstspeicher;



sonderfalle::sonderfalle(string s):
	m(s) {}
char const *sonderfalle::what() const noexcept {
	return this->m.c_str();
}

ZEE::ZEE(h32 k, h32 f):
	sonderfalle(
		(ostringstream() << hex << setfill('0')
		<< "(k = " << setw(8) << k
		<< ",f = " << setw(8) << f
		<< ')').str()
	), _k(k), _f(f) {}
ZZE::ZZE(h32 k, h32 f, zugriff e, int l):
	sonderfalle(
		(ostringstream() << hex << setfill('0')
		<< "(k = " << setw(8) << k
		<< ",f = " << setw(8) << f
		<< ")(z = " << zugriff_wb[e]
		<< ",e = " << setw(3) << bitset<3>(l)
		<< ')').str()
	), _k(k), _f(f), _e(e), _l(l) {}
ZSW::ZSW(h32 k, h32 f, h32 w):
	sonderfalle(
		(ostringstream() << hex << setfill('0')
		<< "(k = " << setw(8) << k
		<< ",f = " << setw(8) << f
		<< ')').str()
	), _k(k), _f(f), _w(w) {}
AUA::AUA(h32 k, h32 f, h64 a):
	sonderfalle(
		(ostringstream() << hex << setfill('0')
		<< "(k = " << setw(8) << k
		<< ",f = " << setw(8) << f
		<< ")(a = " << setw(16) << a
		<< ')').str()
	), _k(k), _f(f), _a(a) {}
