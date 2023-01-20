#include <kunstspeicher.h>
#include <platform.h>
#include <trace.h>
#include "recheneinheit.h"
#include "verbindung.h"
#include "wahrspeicher.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
using namespace kunstspeicher;



istream &cp_getline(istream &i, string &s) {
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



namespace host {

struct host_error_category: public error_category {
	char const *name() const noexcept override final {
		return "Zuse Host Error";
	}
#if defined(ZUSE_POSIX)
	string message(int c) const override final {
		return system_category().message(c);
	}
#elif defined(ZUSE_WINDOWS)
	string message(int c) const override final {
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
};
host_error_category const &error_category() {
	static host_error_category e;
	return e;
}


string no_such_address(h32 a, h32 g) {
	return (ostringstream() << "!! " << setfill('0') << hex << setw(8) << a << " > " << setw(8) << g << '\n').str();
}
string no_such_port(h32 a) {
	return (ostringstream() << "!! !" << setfill('0') << hex << setw(8) << a << '\n').str();
}
template <class type> string format(h32 as, type ag) {
	return (ostringstream() << "   " << setfill('0') << hex << setw(8) << as << " : " << setw(2*sizeof(type)) << ag << '\n').str();
}
template <> string format(h32 as, h8 ag) {
	return (ostringstream() << "   " << setfill('0') << hex << setw(8) << as << " : " << setw(2) << static_cast<unsigned>(ag) << '\n').str();
}

void console(ostream &o, istream &i, wahrspeicher &hs) {
	string c;
	while (o << ">> " && !cp_getline(i, c).eof()) {
		enum class anweisung {g, e, l, s, an, ab};
		unordered_map<string, anweisung> aw({
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
}



int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	if (argc <= 2) {
		cerr << string("Usage:  ").append(argv[0]).append("  $Config_File_Name  $Device_Plugin_Directory/").c_str() << '\n';
		return 1;
	}

	// Parse config file
	ifstream cnf(argv[1], ios_base::binary);
	if (!cnf) {
		cerr << string(argv[1]).append(": ").append(make_error_code(static_cast<errc>(errno)).message()).c_str() << '\n';
		return 2;
	}
	cnf.exceptions(ios::badbit);
	unsigned s;
	cnf >> s; TRACE((ostringstream() << "s = " << dec << s).str().c_str());
	cnf >> ws;

	vector<durchgangeinheit::verbindung> v;
	{
	wahrspeicher hs(s);
	for (string l; cp_getline(cnf, l);) {
		if (l.empty())
			continue;
		istringstream il(l);
		il.exceptions(ios::badbit | ios::failbit);
		char p;
		h32 u;
		il >> p >> hex >> u;
		switch (p) {
		case 'r': {
			TRACE((ostringstream() << "u = " << u).str().c_str());
			hs.ute(u, make_shared<recheneinheit>(hs));
			break;
		}
		case 'd': {
			string n;
			il >> n;
			n = string(argv[2]).append(n).append(ZUSE_PLUGIN_SUFFIX); TRACE(string("n = ").append(n).c_str());
			v.emplace_back(n);
			hs.ute(u, v.back().abb(hs, il));
			break;
		}
		default: {
			cerr << "Unbekannte Einheitsart: " << p << '\n';
		}
		}
	}

	// Create initial page table
	{
		h32 g(s1::z & (hs.g() - 1)), l2(feld<1,10>(g)), l1(feld<11,20>(g));
		for (h32 i(0); i < l2; i += 1) {
			hs.s(g + (i << 2), (i << 22) | stelle<11>(-1) | ss::s | ss::l | ss::a);
		}
		h32 s2(l2 << 22);
		hs.s(g + (l2 << 2), s2 | stelle<21>(-1));
		for (h32 i(0); i < l1; i += 1) {
			hs.s(s2 + (i << 2), s2 | (i << 12) | stelle<21>(-1) | ss::s | ss::l | ss::a);
		}
	}

	// Operator's console
	host::console(cout, cin, hs);
	}

	return 0;
}
