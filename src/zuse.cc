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
		cerr << "Verwendung:  " << argv[0] << "  $Konfig-Datei-Name  $Gerät-Verzeichnis/\n";
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
			if (l.empty())
				continue;
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
				auto m(dlopen((string(argv[2]) + n).c_str(), RTLD_LAZY));
				char *e;
				if (e = dlerror()) {
					TRACE("Ladung ist gescheitert: " + string(e));
					return 1;
				}
				durchgangeinheit::gerat &&(*abb)(istringstream const &);
				*(void **)(&abb) = dlsym(m, "abb");
				if (e = dlerror()) {
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
