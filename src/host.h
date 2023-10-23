#ifndef   ZUSE_HOST_H
#define   ZUSE_HOST_H



// Copyright 2020, 2022- Daniel Campos do Nascimento

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



/* Host interface definitions */

#include <hauptspeicher.h>
#include <platform.h>
#include "durchgangeinheit.h"

#include <iostream>
#include <string>

#if defined(ZUSE_POSIX)
#include <dlfcn.h>
using buchse = void *;
#elif defined(ZUSE_WINDOWS)
#include <Windows.h>
using buchse = HMODULE;
#endif

namespace host {
	struct host_error_category: public std::error_category {
		char const *name() const noexcept override final;
		std::string message(int c) const override final;
	};
	host_error_category const &error_category();

	std::istream &getline(std::istream &i, std::string &s);

	namespace console {
		std::string no_such_address(h32 a, h32 g);
		std::string no_such_port(h32 a);
		template <class type> std::string format(h32 as, type ag);
		void loop(std::ostream &o, std::istream &i, wahrspeicher &hs);
	}

	struct plugin {
		explicit plugin(std::string n);
		void zs(void);

		~plugin();
		plugin();
		plugin(plugin &&);
		plugin &operator=(plugin &&);

		std::shared_ptr<durchgangeinheit> abb(wahrspeicher &hs, std::istringstream &i);

	private:
		std::string n;
		buchse b;
		durchgangeinheit *(*v_ab)(wahrspeicher &hs, std::istringstream &i);
		void (*v_zs)(durchgangeinheit *);
	};
}



#endif /* ZUSE_HOST_H */
