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

	fstream cnf(argv[1], ios_base::in);
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
				il >> n; TRACE(string("n = ").append(n).c_str());
				hs.ute(u, make_shared<durchgangeinheit>(hs, vb(string(argv[2]).append(n).c_str(), il)));
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
