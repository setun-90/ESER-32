#include "wahrspeicher.h"

#include "einheit.h"

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
template <class type> void wahrspeicher::s(h32 w, type a) {
	h8 b[sizeof a];
	for (size_t i(0); i < sizeof a; i++)
		b[i] = static_cast<h8>(a >> (((sizeof a) - i - 1)*CHAR_BIT));
	lock_guard<mutex> l(this->m);
	memcpy(this->hs.data() + w, b, sizeof a);
}
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
template void wahrspeicher::s(h32,   h8);
template void wahrspeicher::s(h32,   h16);
template void wahrspeicher::s(h32,   h32);
template void wahrspeicher::s(h32,   h64);

// Lesen
template <class type> void wahrspeicher::l(type &a, h32 w) {
	h8 b[sizeof a];
	{
		lock_guard<mutex> l(this->m);
		memcpy(b, this->hs.data() + w, sizeof a);
	}
	a = 0;
	for (size_t i(0); i < sizeof a; i++)
		a |= static_cast<type>(b[i]) << (((sizeof a) - i - 1)*CHAR_BIT);
}
template <> void wahrspeicher::l(h8 &a, h32 w) {
	lock_guard<mutex> n(this->m);
	a = this->hs[w];
}
size_t wahrspeicher::g() {
	return this->hs.size();
}
template void wahrspeicher::l(h8 &,  h32);
template void wahrspeicher::l(h16 &, h32);
template void wahrspeicher::l(h32 &, h32);
template void wahrspeicher::l(h64 &, h32);
