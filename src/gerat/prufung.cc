#include "prufung.h"

using namespace std;



unique_ptr<durchgangeinheit::gerat> abb(istringstream &) {
	return unique_ptr<prufung>(new prufung());
}

h32 prufung::operator()(durchgangeinheit *, h64) {
	return 4;
}

void prufung::s(durchgangeinheit *d, h32 z, h32 ab) {
	for (h32 i(0); i < z; i++)
		d->s(ab + i, this->e());
}
void prufung::l(durchgangeinheit *, h32 z, h32) {
	for (h32 i(0); i < z; i++)
		this->e.discard(1);
}
