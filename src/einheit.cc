#include "einheit.h"

using namespace std;

h16 vzw(h16 n, h8 p) {
	return n | ~((n & (1_16 << p)) - 1);
}
h32 vzw(h32 n, h8 p) {
	return n | ~((n & (1_32 << p)) - 1);
}
h64 vzw(h64 n, h8 p) {
	return n | ~((n & (1_64 << p)) - 1);
}

einheit::einheit(wahrspeicher &e):
	se(e), ss(false), ube(einheit::nube) {}

void einheit::ub(h64 e) {
	if (!this->ss)
		return;
	{
		lock_guard<mutex> l(this->m);
		this->ube = e;
	}
	this->cv.notify_one();
}

void einheit::an(void) {
	if (this->ss)
		return;
	this->ss = true;
	this->t = thread(&einheit::operator(), this);
	this->cv.notify_one();
}

void einheit::ab(void) {
	if (!this->ss)
		return;
	{
		lock_guard<mutex> l(this->m);
		this->ss = false;
	}
	this->cv.notify_one();
	this->t.join();
}
