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
durchgangeinheit::verbindung::verbindung(string n):
	n(n) {
	this->b = dlopen(n.c_str(), RTLD_NOW);
	char *f;
	if (!this->b && (f = dlerror()))
		throw runtime_error(string("Ladung ist gescheitert: ").append(f).c_str());
	this->a = reinterpret_cast<shared_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(dlsym(this->b, "abb"));
	if (!this->a && (f = dlerror())) {
		this->zs();
		throw runtime_error(string("Anschalt ist gescheitert: ").append(f).c_str());
	}
}
void durchgangeinheit::verbindung::zs(void) {
	this->a = nullptr;
	if (this->b) {
		dlclose(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#elif defined(ZUSE_WINDOWS)
static char *format_error(void) {
	char *f(nullptr);
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)f,
		0,
		nullptr
	);
	return f;
}

durchgangeinheit::verbindung::verbindung(string n):
	n(n) {
	this->b = LoadLibrary((LPCWSTR)n.c_str());
	if (!this->b)
		throw runtime_error(string("Ladung ist gescheitert: ").append(format_error()).c_str());
	this->a = reinterpret_cast<shared_ptr<durchgangeinheit> (*)(wahrspeicher &, istringstream &)>(GetProcAddress(this->b, (LPCSTR)MAKEINTRESOURCE(1)));
	if (!this->a) {
		this->zs();
		throw runtime_error(string("Anschalt ist gescheitert: ").append(format_error()).c_str());
	}
}
void durchgangeinheit::verbindung::zs(void) {
	this->a = nullptr;
	if (this->b) {
		FreeLibrary(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#endif
durchgangeinheit::verbindung::verbindung():
	n(), b(nullptr), a(nullptr) {}
durchgangeinheit::verbindung::verbindung(verbindung &&v):
	verbindung() {
	*this = move(v);
}
durchgangeinheit::verbindung &durchgangeinheit::verbindung::operator=(verbindung &&v) {
	this->n = move(v.n);
	this->b = v.b;
	this->a = v.a;
	v.a = nullptr;
	v.b = nullptr;
	return *this;
}

shared_ptr<durchgangeinheit> durchgangeinheit::verbindung::abb(wahrspeicher &hs, std::istringstream &i) {
	return this->a(hs, i);
}
durchgangeinheit::verbindung::~verbindung() {
	this->zs();
}
