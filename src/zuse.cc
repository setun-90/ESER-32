#include "recheneinheit.h"
#include "durchgangeinheit.h"
#include "wahrspeicher.h"
#include <trace.h>
#include <dlfcn.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



int main(int argc, char **argv) {
	std::ios_base::sync_with_stdio(false);
	if (argc <= 1) {
		std::cerr << "Verwendung: " << argv[0] << " $Konfig-Datei-Name\n";
		return 1;
	}

	std::fstream cnf(argv[1], std::ios_base::in);
	unsigned s;
	cnf >> s; TRACE("s = " + std::to_string(s));
	wahrspeicher hs(s);
	{
		std::string l;
		cnf >> std::ws;
		while (std::getline(cnf, l)) {
			std::istringstream il(l);
			char p;
			unsigned u;
			il >> p >> std::hex >> u;
			switch (p) {
			case 'r': {
				hs.ute(u, std::make_shared<recheneinheit>(hs));
				break;
			}
			case 'd': {
				il.clear();
				std::string n;
				il >> n;
				TRACE("n = " + n);
				auto m(dlopen(n.c_str(), RTLD_LAZY));
				auto e(dlerror());
				if (e) {
					TRACE("Ladung ist gescheitert: " + std::string(e));
					return 1;
				}
				std::unique_ptr<durchgangeinheit::gerat> &&(*abb)(std::istringstream const &);
				*(void **)(&abb) = dlsym(m, "abb");
				e = dlerror();
				if (e) {
					TRACE("Anschalt ist gescheitert: " + std::string(e));
					return 1;
				}

				hs.ute(u, std::make_shared<durchgangeinheit>(hs, abb(il)));
				break;
			}
			default: {
				std::cerr << "Unbekannte Einheitsart\n";
			}
			}
		}
	}

	return 0;
}
