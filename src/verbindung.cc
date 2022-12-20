#include "durchgangeinheit.h"

#include <string>

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
#include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

using namespace std;



unique_ptr<durchgangeinheit::gerat> durchgangeinheit::gerat::vb(char const *n, istringstream &i) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
	auto m(dlopen(n, RTLD_LAZY));
	char *f;
	if ((f = dlerror()))
		throw runtime_error(string("Ladung ist gescheitert: ").append(f).c_str());
	unique_ptr<durchgangeinheit::gerat> (*abb)(istringstream &)
		(reinterpret_cast<unique_ptr<durchgangeinheit::gerat> (*)(istringstream &)>(dlsym(m, "abb")));
	if ((f = dlerror()))
		throw runtime_error(string("Anschalt ist gescheitert: ").append(f).c_str());
#elif defined(_WIN32) || defined(_WIN64)
	auto m(LoadLibrary(n));
	if (!m)
		throw runtime_error("Ladung ist gescheitert");
	unique_ptr<durchgangeinheit::gerat> (*abb)(istringstream &)
		(reinterpret_cast<unique_ptr<durchgangeinheit::gerat> (*)(istringstream &)>(GetProcAddress(m, "abb")));
	if (!abb)
		throw runtime_error("Anschalt ist gescheitert");
#endif
	return abb(i);
}
