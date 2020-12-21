#include <kunstspeicher.h>

using namespace kunstspeicher;

std::ostream &operator<<(std::ostream &s, zugriff z) {
	switch (z) {
	case zugriff::s: s << 's'; break;
	case zugriff::l: s << 'l'; break;
	case zugriff::a: s << 'a'; break;
	case zugriff::g: s << 'g'; break;
	}
	return s;
}
