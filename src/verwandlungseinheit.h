#ifndef   _VWE_H
#define   _VWE_H



/* Überschrift hier */

#include <kunstspeicher.h>
#include "wahrspeicher.h"

#include <unordered_map>
#include <utility>
#include <functional>
#include <climits>

using umstand = std::pair<h32, h32>;

template <> struct std::hash<umstand> {
	std::hash<h32> h;
	size_t operator()(umstand const &k) const {
		if (sizeof(size_t) == 8)
			return (static_cast<size_t>(k.first) << 32) | static_cast<size_t>(k.second);
		else
			return 3*h(k.first) + h(k.second);
	}
};

using namespace kunstspeicher;

struct verwandlungseinheit {
	verwandlungseinheit(wahrspeicher &s): hs(s) {}

	template <class type> void s(h32 k, h32 g, type a) {
		h32 w[sizeof a];
		this->vwl(w, k, sizeof a, g, zugriff::s);
		for (size_t i(0); i < sizeof a; i++)
			this->hs.s(w[i], static_cast<h8>(a >> (((sizeof a) - i - 1)*CHAR_BIT)));
	}
	template <class type> void l(type &a, h32 k, h32 g) {
		h32 w[sizeof a];
		this->vwl(w, k, sizeof a, g, zugriff::l);
		h8 b;
		a = 0;
		for (size_t i(0); i < sizeof a; i++) {
			this->hs.l(b, w[i]);
			a |= static_cast<type>(b) << (((sizeof a) - i - 1)*CHAR_BIT);
		}
	}
	template <class type> void a(type &a, h32 k, h32 g) {
		h32 w[sizeof a];
		this->vwl(w, k, sizeof a, g, zugriff::a);
		h8 b;
		a = 0;
		for (size_t i(0); i < sizeof a; i++) {
			this->hs.l(b, w[i]);
			a |= static_cast<type>(b) << (((sizeof a) - i - 1)*CHAR_BIT);
		}
	}

	void g(h64 &a, h32 k, h32 g);

	void enk(h32 k, h32 g);

private:
	wahrspeicher &hs;

	std::unordered_map<umstand, h32> s1f, s2f;

	void vwl(h32 *w, h32 k, size_t l, h32 g, zugriff z);
	h32 s2zg(h32 e, zugriff z, h32 k, h32 g);
	h32 s1zg(h32 e, zugriff z, h32 k, h32 g);
};



#endif /* _VWE_H */
