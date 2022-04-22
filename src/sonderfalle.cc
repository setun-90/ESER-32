#include <sonderfalle.h>

#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;
using namespace kunstspeicher;



char const *sonderfalle::what() const noexcept {
	return this->m.c_str();
}

char zu_char(zugriff z) {
	switch (z) {
	case zugriff::s: return 's';
	case zugriff::l: return 'l';
	case zugriff::a: return 'a';
	case zugriff::g: return 'g';
	}
}

ZEE::ZEE(h32 k, h32 f):
	sonderfalle(), _k(k), _f(f) {
	this->m = (ostringstream() << hex << setfill('0')
	<< "(k = " << setw(8) << this->_k
	<< ",f = " << setw(8) << this->_f
	<< ')').str();
}
ZZE::ZZE(h32 k, h32 f, zugriff e, int l):
	sonderfalle(), _k(k), _f(f), _e(e), _l(l) {
	this->m = (ostringstream() << hex << setfill('0')
	<< "(k = " << setw(8) << this->_k
	<< ",f = " << setw(8) << this->_f
	<< ")(z = " << zu_char(this->_e)
	<< ",e = " << setw(3) << bitset<3>(this->_l)
	<< ')').str();
}
ZSW::ZSW(h32 k, h32 f, h32 w):
	sonderfalle(), _k(k), _f(f), _w(w) {
	this->m = (ostringstream() << hex << setfill('0')
	<< "(k = " << setw(8) << this->_k
	<< ",f = " << setw(8) << this->_f
	<< ')').str();
}
AUA::AUA(h32 k, h32 f, h64 a):
	sonderfalle(), _k(k), _f(f), _a(a) {
	this->m = (ostringstream() << hex << setfill('0')
	<< "(k = " << setw(8) << this->_k
	<< ",f = " << setw(8) << this->_f
	<< ")(a = " << setw(16) << this->_a
	<< ')').str();
}
