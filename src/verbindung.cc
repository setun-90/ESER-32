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
	this->v_ab = reinterpret_cast<durchgangeinheit *(*)(wahrspeicher &, istringstream &)>(dlsym(this->b, "abb"));
	this->v_zs = reinterpret_cast<void (*)(durchgangeinheit *)>(dlsym(this->b, "zes"));
	if (!(this->v_ab && this->v_zs) && (f = dlerror())) {
		this->zs();
		throw runtime_error(string("Anschalt ist gescheitert: ").append(f).c_str());
	}
}
void durchgangeinheit::verbindung::zs(void) {
	this->v_zs = nullptr;
	this->v_ab = nullptr;
	if (this->b) {
		dlclose(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#elif defined(ZUSE_WINDOWS)
static TCHAR const *format_error(HRESULT h) {
	LPTSTR f(nullptr);
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		h,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&f,
		0,
		nullptr
	);
	return f ? f : _T("FormatMessage - no error string available");
}

durchgangeinheit::verbindung::verbindung(string n):
	n(n) {
	this->b = LoadLibrary((LPCTSTR)n.c_str());
	if (!this->b) {
		throw runtime_error(string("Ladung ist gescheitert: ").append(format_error(GetLastError())).c_str());
	}
	this->v_ab = reinterpret_cast<durchgangeinheit *(*)(wahrspeicher &, istringstream &)>(GetProcAddress(this->b, "abb"));
	this->v_zs = reinterpret_cast<void (*)(durchgangeinheit *)>(GetProcAddress(this->b, "zes"));
	if (!this->v_ab || !this->v_zs) {
		this->zs();
		throw runtime_error(string("Anschalt ist gescheitert: ").append(format_error(GetLastError())).c_str());
	}
}
void durchgangeinheit::verbindung::zs(void) {
	this->v_zs = nullptr;
	this->v_ab = nullptr;
	if (this->b) {
		FreeLibrary(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#endif
durchgangeinheit::verbindung::verbindung():
	n(), b(nullptr), v_ab(nullptr), v_zs(nullptr) {}
durchgangeinheit::verbindung::verbindung(verbindung &&v):
	verbindung() {
	*this = move(v);
}
durchgangeinheit::verbindung &durchgangeinheit::verbindung::operator=(verbindung &&v) {
	this->n = move(v.n);
	this->b = v.b;
	this->v_ab = v.v_ab;
	this->v_zs = v.v_zs;
	v.v_zs = nullptr;
	v.v_ab = nullptr;
	v.b = nullptr;
	return *this;
}

shared_ptr<durchgangeinheit> durchgangeinheit::verbindung::abb(wahrspeicher &hs, std::istringstream &i) {
	return shared_ptr<durchgangeinheit>(this->v_ab(hs, i), this->v_zs);
}
durchgangeinheit::verbindung::~verbindung() {
	this->zs();
}
