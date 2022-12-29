#include <kunstspeicher.h>
#include <platform.h>
#include <trace.h>
#include "recheneinheit.h"
#include "verbindung.h"
#include "wahrspeicher.h"

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using namespace kunstspeicher;



#if defined(ZUSE_POSIX)
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t handling = 0;

void handle_signal(int s) {
	switch (s) {
	case SIGINT:
	case SIGTERM: {
		if (handling)
			raise(s);
		handling = 1;
		signal(s, SIG_DFL);
		raise(s);
		break;
	}
	default:
		exit(s);
	}
}
#endif



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



int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	if (argc <= 1) {
		cerr << string("Usage:  ").append(argv[0]).append("  $Config_File_Name  $Device_Plugin_Directory/").c_str() << '\n';
		return 1;
	}

	// Parse config file
	ifstream cnf(argv[1]);
	cnf.exceptions(ios::badbit);
	unsigned s;
	cnf >> s; TRACE((ostringstream() << "s = " << dec << s).str().c_str());
	cnf >> ws;

	wahrspeicher hs(s);
	vector<durchgangeinheit::verbindung> v;
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
			il >> n; TRACE(string("n = ").append(n).c_str());
			v.emplace_back(hs, string(argv[2]).append(n).append(".so").c_str(), il);
			hs.ute(u, v.back().ab());
			break;
		}
		default: {
			cerr << "Unbekannte Einheitsart: " << p << '\n';
		}
		}
	}

	// Create initial page table
	{
		h32 g(s1::z & hs.g()), l2(feld<1,10>(g)), l1(feld<11,20>(g));
		for (h32 i(0); i < l2; i += 1) {
			hs.s(g + (i << 2), (i << 22) | stelle<11>(-1) | ss::s | ss::l | ss::a);
		}
		h32 s2(l2 << 22);
		hs.s(g + (l2 << 2), s2 | stelle<21>(-1));
		for (h32 i(0); i < l1; i += 1) {
			hs.s(s2 + (i << 2), s2 | (i << 12) | stelle<21>(-1) | ss::s | ss::l | ss::a);
		}
	}

	// Handle signals/keyboard
#if defined(ZUSE_POSIX)
	struct sigaction sa;
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = handle_signal;
	sigfillset(&sa.sa_mask);
	for (auto s: {SIGINT, SIGTERM})
		sigaction(s, &sa, NULL);

	string c;
	while (cout << "<< " && !cp_getline(cin, c).eof()) {
		cout << string("   ").append(c) << '\n';
	}
	cout << '\n';
#endif

	return 0;
}
