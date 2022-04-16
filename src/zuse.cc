#include "recheneinheit.h"
#include "durchgangeinheit.h"
#include "wahrspeicher.h"
#include <trace.h>
#include <dlfcn.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	if (argc <= 1) {
		cerr << "Verwendung: " << argv[0] << " $Konfig-Datei-Name\n";
		return 1;
	}

	fstream cnf(argv[1], ios_base::in);
	unsigned s;
	cnf >> s; TRACE("s = " + to_string(s));
	wahrspeicher hs(s);
	{
		string l;
		cnf >> ws;
		while (getline(cnf, l)) {
			istringstream il(l);
			char p;
			unsigned u;
			il >> p >> hex >> u;
			switch (p) {
			case 'r': {
				hs.ute(u, make_shared<recheneinheit>(hs));
				break;
			}
			case 'd': {
				il.clear();
				string n;
				il >> n;
				TRACE("n = " + n);
				auto m(dlopen(n.c_str(), RTLD_LAZY));
				auto e(dlerror());
				if (e) {
					TRACE("Ladung ist gescheitert: " + string(e));
					return 1;
				}
				unique_ptr<durchgangeinheit::gerat> &&(*abb)(istringstream const &);
				*(void **)(&abb) = dlsym(m, "abb");
				e = dlerror();
				if (e) {
					TRACE("Anschalt ist gescheitert: " + string(e));
					return 1;
				}

				hs.ute(u, make_shared<durchgangeinheit>(hs, abb(il)));
				break;
			}
			default: {
				cerr << "Unbekannte Einheitsart: " << p << '\n';
			}
			}
		}
	}

	return 0;
}
