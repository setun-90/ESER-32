#include <trace.h>
#include "recheneinheit.h"
#include "durchgangeinheit.h"
#include "wahrspeicher.h"

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;



int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	if (argc <= 1) {
		cerr << string("Verwendung:  ").append(argv[0]).append("  $Konfig_Datei_Name  $Gerät_Verzeichnis/").c_str() << endl;
		return 1;
	}

	ifstream cnf(argv[1], ios_base::in);
	cnf.exceptions(ios::failbit | ios::badbit);
	unsigned s;
	cnf >> s; TRACE((ostringstream() << "s = " << dec << s).str().c_str());
	wahrspeicher hs(s);
	{
		string l;
		cnf >> ws;
		while (getline(cnf, l)) {
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
				hs.ute(u, make_shared<durchgangeinheit>(hs, vb(string(argv[2]).append(n).append(".so").c_str(), il)));
				break;
			}
			default: {
				cerr << "Unbekannte Einheitsart: " << p << endl;
			}
			}
		}
	}

	return 0;
}
