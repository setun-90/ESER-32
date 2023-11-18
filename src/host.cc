#include <trace.h>
#include "host.h"

#include <iomanip>

using namespace std;



char const *host::host_error_category::name() const noexcept {
	return "Zuse Host Error";
}
#if defined(ZUSE_POSIX)
string host::host_error_category::message(int c) const {
	return system_category().message(c);
}
#elif defined(ZUSE_WINDOWS)
string host::host_error_category::message(int c) const {
	LPTSTR f(nullptr);
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		c,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&f,
		0,
		nullptr
	);
	return f ? f : _T("FormatMessage - no error string available");
}
#endif
host::host_error_category const &host::error_category() {
	static host_error_category e;
	return e;
}


istream &host::getline(istream &i, string &s) {
	s.clear();
	istream::sentry si(i, true);
	streambuf *ss(i.rdbuf());

	for (;;) {
		auto c(ss->sbumpc());
		switch (c) {
		case '\n':
			return i;
		case '\r':
			if (ss->sgetc() == '\n')
				ss->sbumpc();
			return i;
		case streambuf::traits_type::eof():
			if (s.empty())
				i.setstate(ios::eofbit);
			return i;
		default:
			s += static_cast<char>(c);
		}
	}
}



string host::console::no_such_address(h32 a, h32 g) {
	return (ostringstream() << "!! " << setfill('0') << hex << setw(8) << a << " > " << setw(8) << g << '\n').str();
}
string host::console::no_such_port(h32 a) {
	return (ostringstream() << "!! !" << setfill('0') << hex << setw(8) << a << '\n').str();
}
template <class type> string host::console::format(h32 as, type ag) {
	return (ostringstream() << "   " << setfill('0') << hex << setw(8) << as << " : " << setw(2*sizeof(type)) << ag << '\n').str();
}
template <> string host::console::format(h32 as, h8 ag) {
	return (ostringstream() << "   " << setfill('0') << hex << setw(8) << as << " : " << setw(2) << static_cast<unsigned>(ag) << '\n').str();
}

void host::console::loop(ostream &o, istream &i, wahrspeicher &hs) {
	string c;
	while (o << ">> " && !getline(i, c).eof()) {
		enum class anweisung {h, g, e, l, s, an, ab};
		unordered_map<string, anweisung> aw({
			{"?",  anweisung::h},
			{"g",  anweisung::g},
			{"e",  anweisung::e},
			{"l",  anweisung::l},
			{"s",  anweisung::s},
			{"an", anweisung::an},
			{"ab", anweisung::ab},
		});

		istringstream ic(c);
		string a;
		ic >> a;
		auto i(aw.find(a));
		if (i == aw.end()) {
			o << string("?? ").append(c) << '\n';
			continue;
		}
		switch (i->second) {
		case anweisung::h: {
			o << (ostringstream()
				<< "   g\n"
				<< "   e\n"
				<< "   l $size $address\n"
				<< "   s $address $data\n"
				<< "   an $interrupt_port_address\n"
				<< "   ab $interrupt_port_address\n"
				).str().c_str();
			break;
		}
		case anweisung::g: {
			o << (ostringstream() << "   " << setfill('0') << hex << setw(8) << hs.g() - 1 << '\n').str().c_str();
			break;
		}
		case anweisung::e: {
			for (auto const &e: hs.ute())
				o << (ostringstream() << "   " << setfill('0') << hex << setw(8) << e.first << '\n').str().c_str();
			break;
		}
		case anweisung::l: {
			unsigned n;
			h32 an;
			ic >> n >> hex >> an >> dec;
			if (!ic) {
				o << string("?? ").append(c) << '\n';
				continue;
			}
			if (an + n > hs.g() - 1) {
				o << no_such_address(an + n, hs.g() - 1).c_str();
				break;
			}
			switch (n) {
			case 8:
			case 7:
			case 6:
			case 5: {
				h64 ab;
				hs.l(ab, an);
				o << format(an, ab).c_str();
				break;
			}
			case 4:
			case 3: {
				h32 ab;
				hs.l(ab, an);
				o << format(an, ab).c_str();
				break;
			}
			case 2: {
				h16 ab;
				hs.l(ab, an);
				o << format(an, ab).c_str();
				break;
			}
			case 1: {
				h8 ab;
				hs.l(ab, an);
				o << format(an, ab).c_str();
				break;
			}
			}
			break;
		}
		case anweisung::s: {
			h32 as, ag;
			ic >> hex >> as >> ag;
			if (!ic) {
				o << string("?? ").append(c) << '\n';
				continue;
			}
			if (as + 8 > hs.g() - 1) {
				o << no_such_address(as + 8, hs.g() - 1).c_str();
				break;
			}
			hs.s(as, ag);
			o << format(as, ag).c_str();
			break;
		}
		case anweisung::an: {
			h32 ut;
			ic >> hex >> ut;
			if (!ic) {
				o << string("?? ").append(c) << '\n';
				continue;
			}
			if (ut > hs.g() - 1) {
				o << no_such_address(ut, hs.g() - 1).c_str();
				break;
			}
			auto e(hs.ute().find(ut));
			if (e == hs.ute().end()) {
				o << no_such_port(ut).c_str();
				break;
			}
			e->second->an();
			break;
		}
		case anweisung::ab: {
			h32 ut;
			ic >> hex >> ut;
			if (!ic) {
				o << string("?? ").append(c) << '\n';
				continue;
			}
			if (ut > hs.g() - 1) {
				o << no_such_address(ut, hs.g() - 1).c_str();
				break;
			}
			auto e(hs.ute().find(ut));
			if (e == hs.ute().end()) {
				o << no_such_port(ut).c_str();
				break;
			}
			e->second->ab();
			break;
		}
		}
	}
	o << '\n';
}

#if defined(ZUSE_POSIX)
host::plugin::plugin(string n):
	n(n) {
	this->b = dlopen(n.c_str(), RTLD_NOW);
	char *f;
	if (!this->b && (f = dlerror()))
		throw runtime_error(f);
	this->v_ab = reinterpret_cast<durchgangeinheit *(*)(wahrspeicher &, istringstream &)>(dlsym(this->b, "abb"));
	this->v_zs = reinterpret_cast<void (*)(durchgangeinheit *)>(dlsym(this->b, "zes"));
	if (!(this->v_ab && this->v_zs) && (f = dlerror())) {
		this->zs();
		throw runtime_error(f);
	}
}
void host::plugin::zs(void) {
	this->v_zs = nullptr;
	this->v_ab = nullptr;
	if (this->b) {
		dlclose(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#elif defined(ZUSE_WINDOWS)
host::plugin::plugin(string n):
	n(n) {
	this->b = LoadLibrary((LPCTSTR)n.c_str());
	if (!this->b) {
		throw runtime_error(error_code(GetLastError(), host::error_category()).message());
	}
	this->v_ab = reinterpret_cast<durchgangeinheit *(*)(wahrspeicher &, istringstream &)>(GetProcAddress(this->b, "abb"));
	this->v_zs = reinterpret_cast<void (*)(durchgangeinheit *)>(GetProcAddress(this->b, "zes"));
	if (!this->v_ab || !this->v_zs) {
		this->zs();
		throw runtime_error(error_code(GetLastError(), host::error_category()).message());
	}
}
void host::plugin::zs(void) {
	this->v_zs = nullptr;
	this->v_ab = nullptr;
	if (this->b) {
		FreeLibrary(this->b);
		this->b = nullptr;
	}
	this->n.clear();
}
#endif
host::plugin::plugin():
	n(), b(nullptr), v_ab(nullptr), v_zs(nullptr) {}
host::plugin::plugin(plugin &&v):
	plugin() {
	*this = move(v);
}
host::plugin &host::plugin::operator=(plugin &&v) {
	this->n = move(v.n);
	this->b = v.b;
	this->v_ab = v.v_ab;
	this->v_zs = v.v_zs;
	v.v_zs = nullptr;
	v.v_ab = nullptr;
	v.b = nullptr;
	return *this;
}

shared_ptr<durchgangeinheit> host::plugin::abb(wahrspeicher &hs, std::istringstream &i) {
	durchgangeinheit *p(this->v_ab(hs, i));
	if (!p)
		throw system_error(errno, host::error_category());
	return shared_ptr<durchgangeinheit>(p, this->v_zs);
}
host::plugin::~plugin() {
	this->zs();
}
