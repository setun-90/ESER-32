#ifndef   _SF_H
#define   _SF_H



/* EZER Sonderfalle Begriffserklärungen */

#include "kunstspeicher.h"

#include <exception>
#include <sstream>
#include <iomanip>

class sonderfalle: public std::exception {
protected:
	std::string m;
};

class ZEE: public sonderfalle {
	h32 _k, _f;
public:
	ZEE(h32 k, h32 f);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }

	char const *what() const noexcept { return this->m.c_str(); }
};
class ZZE: public sonderfalle {
	h32 _k, _f;
	kunstspeicher::zugriff _e;
	int _l;
public:
	ZZE(h32 k, h32 f, kunstspeicher::zugriff e, int l);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	kunstspeicher::zugriff e(void) { return this->_e; }
	h32 l(void) { return this->_l; }

	char const *what() const noexcept { return this->m.c_str(); }
};
class ZSW: public sonderfalle {
	h32 _k, _f, _w;
public:
	ZSW(h32 k, h32 f, h32 w);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	h32 w(void) { return this->_w; }

	char const *what() const noexcept { return this->m.c_str(); }
};
class AUA: public sonderfalle {
	h32 _k, _f;
	h64 _a;
public:
	AUA(h32 k, h32 f, h64 a);

	h32 k(void) { return this->_k; }
	h32 f(void) { return this->_f; }
	h64 a(void) { return this->_a; }

	char const *what() const noexcept { return this->m.c_str(); }
};



#endif /* _SF_H */
