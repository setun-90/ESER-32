#include "prufung.h"

using namespace std;



prufung abb(istringstream &) {
	return prufung();
}

h32 prufung::operator()(durchgangeinheit *, h64) {
	return 4;
}

void prufung::s(durchgangeinheit *d, h32 z, h32 ab) {
	for (size_t i(0); i < z; i++)
		d->s(ab + i, this->e());
}
void prufung::l(durchgangeinheit *, h32 z, h32) {
	for (size_t i(0); i < z; i++)
		this->e.discard(1);
}
