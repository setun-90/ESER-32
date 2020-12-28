#ifndef   _DE_H
#define   _DE_H



/* EZER Durchgangeinheitsentwurf */
/*     (C) 1979, 1988, 1992      */

#include "einheit.h"
#include "wahrspeicher.h"
#include <unterbrechung.h>
#include <kunstspeicher.h>

#include <utility>
#include <memory>

struct durchgangeinheit: public einheit {
	struct gerat {
		virtual void operator()(durchgangeinheit *d, h64 &a) = 0;
		virtual void s(durchgangeinheit *d, h32 z, h32 ab)   = 0;
		virtual void l(durchgangeinheit *d, h32 z, h32 ab)   = 0;
	};

	durchgangeinheit(wahrspeicher &e, std::unique_ptr<gerat> &&g);

	void operator()(void) override;

	template <class art> void s(h32 k, art a) {
		this->se.s(k, this->gfb, a);
	}
	template <class art> void l(art &a, h32 k) {
		this->se.l(a, k, this->gfb);
	}
	template <class art> void a(art &a, h32 k) {
		this->se.a(a, k, this->gfb);
	}

private:
	void af(h64 a);
	void ubv(void);

	h32 az;
	h32 gfb;
	h32 utz;
	h32 uez;
	h8  b;
	bool zs;

	std::unique_ptr<gerat> gr;
};



#endif /* _DE_H */
