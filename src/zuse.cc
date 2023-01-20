#include <kunstspeicher.h>
#include <platform.h>
#include <trace.h>
#include "recheneinheit.h"
#include "wahrspeicher.h"
#include "host.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
using namespace kunstspeicher;



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

	vector<host::plugin> v;
	{
	wahrspeicher hs(s);
	for (string l; host::getline(cnf, l);) {
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
			cerr << "Unknown unit type: " << p << '\n';
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
	host::console::loop(cout, cin, hs);
	}

	return 0;
}
