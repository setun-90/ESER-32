#include <sonderfalle.h>
#include <iomanip>
#include <bitset>

using namespace std;
using namespace kunstspeicher;

char const *sonderfalle::what() const noexcept {
	return this->m.str().c_str();
}

ostream &operator<<(ostream &s, zugriff z) {
	switch (z) {
	case zugriff::s: s << 's'; break;
	case zugriff::l: s << 'l'; break;
	case zugriff::a: s << 'a'; break;
	case zugriff::g: s << 'g'; break;
	}
	return s;
}

ZEE::ZEE(h32 k, h32 f):
	sonderfalle(), _k(k), _f(f) {
	this->m << hex << setfill('0');
	this->m << "(k = " << setw(8) << this->_k;
	this->m << ",f = " << setw(8) << this->_f;
	this->m << ')';
}
ZZE::ZZE(h32 k, h32 f, zugriff e, int l):
	sonderfalle(), _k(k), _f(f), _e(e), _l(l) {
	this->m << hex << setfill('0');
	this->m << "(k = " << setw(8) << this->_k;
	this->m << ",f = " << setw(8) << this->_f;
	this->m << ")(z = " << this->_e;
	this->m << ",e = " << setw(3) << bitset<3>(this->_l);
	this->m << ')';
}
ZSW::ZSW(h32 k, h32 f, h32 w):
	sonderfalle(), _k(k), _f(f), _w(w) {
	this->m << hex << setfill('0');
	this->m << "(k = " << setw(8) << this->_k;
	this->m << ",f = " << setw(8) << this->_f;
	this->m << ')';
}
AUA::AUA(h32 k, h32 f, h64 a):
	sonderfalle(), _k(k), _f(f), _a(a) {
	this->m << hex << setfill('0');
	this->m << "(k = " << setw(8) << this->_k;
	this->m << ",f = " << setw(8) << this->_f;
	this->m << ")(a = " << setw(16) << this->_a;
	this->m << ')';
}
