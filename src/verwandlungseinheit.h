#ifndef   _VWE_H
#define   _VWE_H



/* EZER Anschriftverwandlungseinheitsbegriffserklärung */
/*                 (C) 1979, 1988, 1992                */

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
	verwandlungseinheit(wahrspeicher &s);

	template <class type> void s(h32 k, h32 g, type a);
	template <class type> void l(type &a, h32 k, h32 g);
	template <class type> void a(type &a, h32 k, h32 g);
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
