# Commands
RM := rm -rf

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
LD           := ${CXX}

# Items
SRCS := ${shell find src/ prf/ -name '*.cc'}
OBJS := ${SRCS:%.cc=%.o}
KERN := ${filter-out prf/% src/gerat/%,${OBJS}}
GRTE := ${filter src/gerat/%,${OBJS}}
PRFN := ${filter-out src/%,${OBJS:prf/%.o=prf/%}}

.PHONY: all clean

.PRECIOUS: ${OBJS}


all:  zuse gerate prufungen # ${PRFN}

zuse: bin/zuse
ifeq "${CXX}" "g++"
bin/%: CXXFLAGS := ${CXXFLAGS} -O2 -fno-reorder-blocks-and-partition -fno-reorder-functions -fira-region=mixed -ftree-cselim -flive-range-shrinkage -fpredictive-commoning -ftree-loop-distribution -fsched-pressure -fweb -frename-registers -fipa-pta -flto=auto -flto-partition=one -floop-nest-optimize -fgraphite-identity -fno-plt -fno-semantic-interposition -fdevirtualize-at-ltrans
bin/%: LDFLAGS  := ${LDFLAGS},-s,-Bsymbolic,-z,relro,-z,combreloc
else ifeq "${CXX}" "clang++"
bin/%: CXXFLAGS := ${CXXFLAGS} -O2 -flto=thin -fno-plt
bin/%: LDFLAGS  := ${LDFLAGS},-s,-Bsymbolic,-z,relro,-z,combreloc -flto=thin
endif
bin/%: src/%.o ${KERN}
	${LD} ${LDFLAGS} $^ ${LDLIBS} -o $@

gerate: ${GRTE:src/gerat/%.o=lib/%.so}
#lib/%.so: CXXFLAGS := ${CXXFLAGS} -O2 -fno-reorder-blocks-and-partition -fno-reorder-functions -fira-region=mixed -ftree-cselim -flive-range-shrinkage -fpredictive-commoning -ftree-loop-distribution -fsched-pressure -fweb -frename-registers -fipa-pta -flto=auto -flto-partition=one -floop-nest-optimize -fgraphite-identity -fno-plt -fno-semantic-interposition -fdevirtualize-at-ltrans
lib/%.so: LDFLAGS := ${LDFLAGS},-s,-Bsymbolic,-z,relro,-z,combreloc -shared
lib/%.so: src/gerat/%.o ${KERN}
	${LD} ${LDFLAGS} $^ ${LDLIBS} -o $@




prufungen: ${PRFN}
prf/%: CXXFLAGS := ${CXXFLAGS} -Og -ggdb -fsanitize=address
prf/%: LDFLAGS := ${LDFLAGS} -fsanitize=address
prf/%: prf/%.o lib/prufung.so ${filter-out src/zuse.o,${KERN}}
	${LD} ${LDFLAGS} $^ ${LDLIBS} -o $@



clean:
	${RM} ${wildcard bin/*} ${wildcard lib/*} ${PRFN} ${OBJS}
