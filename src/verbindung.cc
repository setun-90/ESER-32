#include "durchgangeinheit.h"
#include <trace.h>

#include <iostream>
#include <dlfcn.h>

using namespace std;



unique_ptr<durchgangeinheit::gerat> vb(char const *n, istringstream &i) {
	auto m(dlopen(n, RTLD_LAZY));
	char *f;
	if ((f = dlerror())) {
		TRACE(string("Ladung ist gescheitert: ").append(f).c_str());
		return nullptr;
	}
	unique_ptr<durchgangeinheit::gerat> (*abb)(istringstream &)
		(reinterpret_cast<unique_ptr<durchgangeinheit::gerat> (*)(istringstream &)>(dlsym(m, "abb")));
	if ((f = dlerror())) {
		TRACE(string("Anschalt ist gescheitert: ").append(f).c_str());
		return nullptr;
	}
	return unique_ptr<durchgangeinheit::gerat>(abb(i));
}
