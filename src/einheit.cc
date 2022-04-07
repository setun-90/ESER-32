#include "einheit.h"

h16 vzw(h16 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}
h32 vzw(h32 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}
h64 vzw(h64 n, h8 p) {
	return n | ~((n & (1 << p)) - 1);
}

void einheit::ub(h64 e) {
	if (!this->t.joinable())
		this->t = std::thread(&einheit::lf, this);
#if __cplusplus >= 202002L
	this->ube = e;
	this->ube.notify_one();
#else
	{
		std::lock_guard<std::mutex> l(this->m);
		this->ube = e;
	}
	this->cv.notify_one();
#endif
}

void einheit::hl(void) {
	this->t.std::thread::~thread();
}
