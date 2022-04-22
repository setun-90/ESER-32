#ifndef   _WS_H
#define   _WS_H



/* EZER Wahrspeicherbegriffserklärung */
/*        (C) 1979, 1988, 1992        */
#include <hauptspeicher.h>

#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstring>
#include <climits>

class einheit;

class wahrspeicher {
	std::vector<h8> hs;
	std::mutex m;
	std::unordered_map<h32, std::shared_ptr<einheit>> ut;

public:
	wahrspeicher(h32 g);

	void ute(h32 a, std::shared_ptr<einheit> e);
	void ute(h32 a);

	template <class type> void s(h32 w, type a);
	template <class type> void l(type &a, h32 w);

	size_t g();
};



#endif /* _WS_H */
