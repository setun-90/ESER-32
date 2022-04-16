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
	se(e), ube(einheit::nube), an(false) {}

void einheit::ub(h64 e) {
	if (!this->an) {
		this->an = true;
		this->t = thread(&einheit::operator(), this);
	}
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
#if __cplusplus >= 202002L
	if (this->an) {
		this->an = false;
		this->an.notify_one();
		this->t.join();
	}
#else
	if (this->an) {
		{
			lock_guard<mutex> l(this->m);
			this->an = false;
		}
		this->cv.notify_one();
		this->t.join();
	}
#endif
}
