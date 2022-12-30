#include "einheit.h"

using namespace std;

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
	this->t.detach();
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
}
