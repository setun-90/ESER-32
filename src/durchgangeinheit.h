#ifndef   _DE_H
#define   _DE_H



/* EZER Durchgangeinheitsentwurf */
/*     (C) 1979, 1988, 1992      */

#include "einheit.h"
#include "wahrspeicher.h"
#include <unterbrechung.h>
#include <kunstspeicher.h>

#include <sstream>

struct durchgangeinheit: public einheit {
	struct gerat {
		virtual ~gerat() = default;
		virtual void operator()(durchgangeinheit *d, h64 &a) = 0;
		virtual void s(durchgangeinheit *d, h32 z, h32 ab)   = 0;
		virtual void l(durchgangeinheit *d, h32 z, h32 ab)   = 0;
	};

private:
	void af(h64 a);
	void ubv(void);
	void operator()(void) override;

	h32 az;
	h32 gfb;
	h32 utz;
	h32 uez;
	h8  b;
	bool zs;

	gerat &&gr;

public:
	durchgangeinheit(wahrspeicher &e, gerat &&g);

	bool ls(void) override;
	template <class art> void s(h32 k, art a);
	template <class art> void l(art &a, h32 k);
	template <class art> void a(art &a, h32 k);
};



#endif /* _DE_H */
