#ifndef   _VE_H
#define   _VE_H



#include <hauptspeicher.h>
#include <unterbrechung.h>
#include "verwandlungseinheit.h"

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

struct einheit {
	virtual ~einheit() = default;
	virtual bool ls(void) = 0;
	void ub(h64 e);
	void as(void);
	static h64 const nube = static_cast<h64>(1) << 32;

protected:
	verwandlungseinheit se;
	virtual void operator()(void) = 0;
	std::atomic<h64> ube;
	std::atomic<bool> an;
	std::thread t;
#if __cplusplus < 202002L
	std::condition_variable cv;
	std::mutex m;
#endif

	einheit(wahrspeicher &e);
	einheit(einheit const &) = delete;
	einheit &operator=(einheit const &) = delete;
	einheit(einheit &&) = default;
	einheit &operator=(einheit &&) = default;
};

h16 vzw(h16 n, h8 p);
h32 vzw(h32 n, h8 p);
h64 vzw(h64 n, h8 p);

template <class type> type feld(h8 p1, h8 p2) {
	return ((static_cast<type>(1) << (p1 - p2 + 1)) - 1) << p2;
}


#endif /* _VE_H */
