#include "prufung.h"

using namespace std;



shared_ptr<durchgangeinheit> abb(wahrspeicher &hs, istringstream &) {
	return make_shared<prufung>(hs);
}

h32 prufung::operator()(h64) {
	return 4;
}

void prufung::s(h32 z, h32 ab) {
	for (h32 i(0); i < z; i += 1)
		this->se.s(ab + i, this->e());
}
void prufung::l(h32 z, h32) {
	for (h32 i(0); i < z; i += 1)
		this->e.discard(1);
}
