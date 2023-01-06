#include "prufung.h"

using namespace std;



durchgangeinheit *abb(wahrspeicher &hs, istringstream &) {
	return new prufung(hs);
}
void zes(durchgangeinheit *d) {
	delete d;
}

h32 prufung::operator()(h64) {
	return 4;
}

void prufung::g_s(h32 z, h32 ab) {
	for (h32 i(0); i < z; i += 1)
		if (!this->s(ab + i, this->e()))
			return;
}
void prufung::g_l(h32 z, h32) {
	for (h32 i(0); i < z; i += 1)
		this->e.discard(1);
}
