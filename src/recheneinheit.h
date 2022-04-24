#ifndef   _RE_H
#define   _RE_H



/* EZER Recheneinheitsentwurf */
/*    (C) 1979, 1988, 1992    */

#include "einheit.h"

#include <utility>
#include <condition_variable>
#include <mutex>

class recheneinheit: public einheit {
	// Anweisungsabteilungen
	static const h8 a_g = 32 - 2, a_b = 32 - 8, a_z = 32 - 12, a_q = 32 - 16, a_an = 32 - 17;
	static const h32 a_w = (1 << (32 - 12)) - 1, a_a = (1 << (32 - 20)) - 1, a_ra = (1 << (32 - 16)) - 1;

	void operator()(void) override;
	void af(void);
	template <class art> void s(h32 k, art a);
	template <class art> void l(art &a, h32 k);
	template <class art> void a(art &a, h32 k);
	void nss(h8 z,  h32 a);
	void nsl(h32 &a, h8 z);
	static h8 zb(h32 a);

	h32 ns[16];
	h32 az;
	h32 gfb;
	h8  b;
	bool zs, zes;

public:
	recheneinheit(wahrspeicher &hs);

	bool ls(void) override;
};



#endif /* _RE_H */
