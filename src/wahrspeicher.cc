#include "einheit.h"
#include "wahrspeicher.h"

using namespace std;

wahrspeicher::wahrspeicher(h32 g):
	hs(g << 12) {}

// Unterbrechungstore
void wahrspeicher::ute(h32 w, shared_ptr<einheit> e) {
	this->ut.emplace(w, e);
}
void wahrspeicher::ute(h32 w) {
	this->ut.erase(w);
}

// Schreiben
template <> void wahrspeicher::s(h32 w, h8 a) {
	lock_guard<mutex> n(this->m);
	this->hs[w] = a;
}
template <> void wahrspeicher::s(h32 w, h64 a) {
	if (this->ut.count(w))
		this->ut[w]->ub(a);
	h8 b[sizeof a] = {
		static_cast<h8>(a >> 56),
		static_cast<h8>(a >> 48),
		static_cast<h8>(a >> 40),
		static_cast<h8>(a >> 32),
		static_cast<h8>(a >> 24),
		static_cast<h8>(a >> 16),
		static_cast<h8>(a >> 8),
		static_cast<h8>(a)
	};
	lock_guard<mutex> n(this->m);
	memcpy(this->hs.data() + w, b, sizeof a);
}

// Lesen
template <> void wahrspeicher::l(h8 &a, h32 w) {
	lock_guard<mutex> n(this->m);
	a = this->hs[w];
}

size_t wahrspeicher::g() {
	return this->hs.size();
}
