#include <platform.h>
#include <trace.h>
#include "verbindung.h"

#include <string>

#if defined(ZUSE_POSIX)
#include <dlfcn.h>
#elif defined(ZUSE_WINDOWS)
#include <Windows.h>
#endif

using namespace std;



#if defined(ZUSE_POSIX)
durchgangeinheit::verbindung::verbindung(wahrspeicher &hs, char const *n, std::istringstream &i) {
	this->b = dlopen(n, RTLD_NOW);
	char *f;
	if ((f = dlerror()))
		throw runtime_error(string("Ladung ist gescheitert: ").append(f).c_str());
	shared_ptr<durchgangeinheit> (*abb)(wahrspeicher &, istringstream &)
		(reinterpret_cast<shared_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(dlsym(this->b, "abb")));
	if ((f = dlerror()))
		throw runtime_error(string("Anschalt ist gescheitert: ").append(f).c_str());
	this->d = abb(hs, i);
}
void durchgangeinheit::verbindung::zs(void) {
	dlclose(this->b);
	char *f;
	if ((f = dlerror()))
		throw runtime_error(string("Abschalt ist gescheitert: ").append(f).c_str());
}
#elif defined(ZUSE_WINDOWS)
static LPTSTR format_error(void) {
	LPTSTR f;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		&f,
		0,
		nullptr
	);
	return f;
}

durchgangeinheit::verbindung::verbindung(wahrspeicher &hs, char const *n, std::istringstream &i) {
	this->b = LoadLibrary(n);
	if (!this->b)
		throw runtime_error(string("Ladung ist gescheitert: ").append(format_error()).c_str());
	shared_ptr<durchgangeinheit> (*abb)(wahrspeicher &, istringstream &)
		(reinterpret_cast<shared_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(GetProcAddress(this->b, "abb")));
	if (!abb)
		throw runtime_error(string("Anschalt ist gescheitert: ").append(format_error()).c_str());
	this->d = abb(hs, i);
}
void durchgangeinheit::verbindung::zs(void) {
	if (!FreeLibrary(this->b))
		throw runtime_error(string("Abschalt ist gescheitert: ").append(format_error()).c_str());
}
#endif

shared_ptr<durchgangeinheit> durchgangeinheit::verbindung::ab(void) {
	return this->d;
}
durchgangeinheit::verbindung::~verbindung() {
	try {
		this->zs();
	} catch (runtime_error const &r) {
		TRACE(r.what());
	}
}
