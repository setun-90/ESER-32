# Commands
RM := rm -rf

# Flags
CXXFLAGS     := -I./include/ -Wall -Wextra -Wpedantic -march=native -pipe -std=c++11 -Wa,-mbranches-within-32B-boundaries -fPIC -falign-functions=32 -falign-jumps=32 -falign-loops=32
LDFLAGS      := -ldl -pthread
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
bin/%: CXXFLAGS := ${CXXFLAGS} -O2 -fno-reorder-blocks-and-partition -fno-reorder-functions -fipa-pta -fno-plt -fno-semantic-interposition -flto=auto -fdevirtualize-at-ltrans -floop-nest-optimize -fgraphite-identity
bin/%: LDFLAGS  := -Wl,-s,-O1,--sort-common,-Bsymbolic,-z,relro,-z,combreloc ${LDFLAGS}
bin/%: src/%.o ${KERN}
	${LD} ${LDFLAGS} $^ -o $@

gerate: ${GRTE:src/gerat/%.o=lib/%.so}
lib/%.so: LDFLAGS := -Wl,-s,-O1,--sort-common,-Bsymbolic,-z,relro,-z,combreloc -shared ${LDFLAGS}
lib/%.so: ${KERN} src/gerat/%.o
	${LD} ${LDFLAGS} $^ -o $@




prufungen: ${PRFN}
prf/%: CXXFLAGS := ${CXXFLAGS} -Og -ggdb
prf/%: LDFLAGS  := -Wl,-O1 ${LDFLAGS}
prf/%: prf/%.o lib/prufung.so ${filter-out src/zuse.o,${KERN}}
	${LD} ${LDFLAGS} $^ -o $@



clean:
	${RM} ${wildcard bin/*} ${wildcard lib/*} ${PRFN} ${OBJS}
