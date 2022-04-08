#include "einheit.h"

using namespace std;

h16 vzw(h16 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}
h32 vzw(h32 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}
h64 vzw(h64 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}

einheit::einheit(wahrspeicher &e):
	se(e), ube(einheit::nube) {}
void einheit::ub(h64 e) {
	if (!this->t.joinable())
		this->t = thread(&einheit::lf, this);
#if __cplusplus >= 202002L
	this->ube = e;
	this->ube.notify_one();
#else
	{
		lock_guard<mutex> l(this->m);
		this->ube = e;
	}
	this->cv.notify_one();
#endif
}

void einheit::as(void) {
	this->t.std::thread::~thread();
}
