#ifndef   _PG_H
#define   _PG_H



/* EZER Prüfungsgerätsbegriffserklärung */
/*        (C) 1979, 1988, 1992          */

#include "../durchgangeinheit.h"

#include <random>

struct prufung: public durchgangeinheit::gerat {
	h32 operator()(durchgangeinheit *d, h64 a) override;
	void s(durchgangeinheit *d, h32 z, h32 ab) override;
	void l(durchgangeinheit *d, h32 z, h32 ab) override;
private:
	std::mt19937_64 e;
};

extern "C" prufung abb(std::istringstream &i);



#endif /* _PG_H */
