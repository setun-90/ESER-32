# Commands
RM    := rm -rf
LINK   = ${LINK.cc} $^ ${LOADLIBES} ${LDLIBS} -o $@

# Flags
CPPFLAGS     := -I./include/
ifeq "${findstring clean,${MAKECMDGOALS}}" ""
ifeq "${CXX}" "g++"
CXXFLAGS     := -Wall -Wextra -Wpedantic -march=native -pipe -std=c++11 -Wa,-mbranches-within-32B-boundaries -mno-direct-extern-access -fPIC -falign-functions=32 -falign-jumps=32 -falign-loops=32
else ifeq "${CXX}" "clang++"
CXXFLAGS     := -Wall -Wextra -Wpedantic -march=native -pipe -std=c++11 -fPIC -fno-direct-access-external-data -falign-functions=32 -falign-loops=32
else
${info Cannot determine compiler, set CXXFLAGS and LDFLAGS for optimizations and linking options}
endif
endif
LDFLAGS      := -Wl,-O1,--as-needed,--sort-common
LDLIBS       := -ldl -pthread

# Items
SRCS := ${shell find src/ prf/ -name '*.cc'}
OBJS := ${SRCS:%.cc=%.o}
KERN := ${filter-out prf/% src/gerat/% src/zuse.o,${OBJS}}
GRTE := ${filter src/gerat/%,${OBJS}}
PRFN := ${filter-out src/%,${OBJS:prf/%.o=prf/%}}

.PHONY: zuse gerate prufungen clean

.PRECIOUS: ${OBJS}



ifeq "${CXX}" "g++"
zuse: CXXFLAGS := ${CXXFLAGS} -O2 -fno-reorder-blocks-and-partition -fno-reorder-functions -fira-region=mixed -ftree-cselim -flive-range-shrinkage -fpredictive-commoning -ftree-loop-distribution -fsched-pressure -fweb -frename-registers -fipa-pta -flto=auto -flto-partition=one -floop-nest-optimize -fgraphite-identity -fno-plt -fno-semantic-interposition -fdevirtualize-at-ltrans
else ifeq "${CXX}" "clang++"
zuse: CXXFLAGS := ${CXXFLAGS} -O2 -flto=thin -fno-plt
endif
zuse: LDFLAGS  := ${LDFLAGS},-s,-Bsymbolic,-z,relro,-z,combreloc
zuse: bin/zuse gerate
bin/%: src/%.o ${KERN}
	${LINK}

gerate: ${GRTE:src/gerat/%.o=lib/%.so}
lib/%.so: LDFLAGS := ${LDFLAGS} -shared
lib/%.so: src/gerat/%.o ${KERN}
	${LINK}




prufungen: ${PRFN}
prf/%: CXXFLAGS := ${CXXFLAGS} -Og -ggdb -fsanitize=address
prf/%: prf/%.o lib/prufung.so ${KERN}
	${LINK}



clean:
	${RM} ${wildcard bin/*} ${wildcard lib/*} ${PRFN} ${OBJS}
