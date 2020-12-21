#ifndef   _WS_H
#define   _WS_H



/* Überschrift hier */
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
	wahrspeicher(h32 g): hs(g << 12) {}

	void ute(h32 a, std::shared_ptr<einheit> e);
	void ute(h32 a);

	template <class type> void s(h32 w, type a) {
		h8 b[sizeof a];
		for (size_t i(0); i < sizeof a; i++)
			b[i] = static_cast<h8>(a >> (((sizeof a) - i - 1)*CHAR_BIT));
		std::lock_guard<std::mutex> l(this->m);
		memcpy(this->hs.data() + w, b, sizeof a);
	}

	template <class type> void l(type &a, h32 w) {
		h8 b[sizeof a];
		{
			std::lock_guard<std::mutex> l(this->m);
			memcpy(b, this->hs.data() + w, sizeof a);
		}
		a = 0;
		for (size_t i(0); i < sizeof a; i++)
			a |= static_cast<type>(b[i]) << (((sizeof a) - i - 1)*CHAR_BIT);
	}

	size_t g();
};



#endif /* _WS_H */
