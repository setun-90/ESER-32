#include "prufung.h"

void prufung::operator()(durchgangeinheit *d, h64 &a) {
	return;
}

void prufung::s(durchgangeinheit *d, h32 z, h32 ab) {
	for (size_t i(0); i < z; i++)
		d->s(ab + i, this->e());
}
void prufung::l(durchgangeinheit *d, h32 z, h32 ab) {
	for (size_t i(0); i < z; i++)
		this->e.discard(1);
}
