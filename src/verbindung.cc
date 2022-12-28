#include <platform.h>
#include "durchgangeinheit.h"

#include <string>

#if defined(ZUSE_POSIX)
#include <dlfcn.h>
#elif defined(ZUSE_WINDOWS)
#include <Windows.h>
#endif

using namespace std;



unique_ptr<durchgangeinheit> durchgangeinheit::vb(wahrspeicher &hs, char const *n, istringstream &i) {
#if defined(ZUSE_POSIX)
	auto m(dlopen(n, RTLD_LAZY));
	char *f;
	if ((f = dlerror()))
		throw runtime_error(string("Ladung ist gescheitert: ").append(f).c_str());
	unique_ptr<durchgangeinheit> (*abb)(wahrspeicher &, istringstream &)
		(reinterpret_cast<unique_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(dlsym(m, "abb")));
	if ((f = dlerror()))
		throw runtime_error(string("Anschalt ist gescheitert: ").append(f).c_str());
#elif defined(ZUSE_WINDOWS)
	auto m(LoadLibrary(n));
	if (!m)
		throw runtime_error("Ladung ist gescheitert");
	unique_ptr<durchgangeinheit> (*abb)(wahrspeicher &, istringstream &)
		(reinterpret_cast<unique_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(GetProcAddress(m, "abb")));
	if (!abb)
		throw runtime_error("Anschalt ist gescheitert");
#endif
	return abb(hs, i);
}
