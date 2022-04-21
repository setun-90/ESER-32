# Commands
RM := rm -rf

# Flags
CXXFLAGS     := -I./include/ -Wall -Wextra -Wpedantic -march=native -pipe -std=c++11 -Wa,-mbranches-within-32B-boundaries -falign-functions=32 -falign-jumps=32 -falign-loops=32
LDFLAGS      := -ldl -pthread
LD           := ${CXX}

# Items
TGT := zuse
TGT-SRC := src/zuse.cc
TGT-OBJ := obj/zuse.o
PRF-SRCS := ${wildcard prf/*.cc}
PRFN := ${PRF-SRCS:%.cc=%}
SRCS := ${wildcard src/*.cc}
OBJS := ${filter-out ${TGT-OBJ},${SRCS:src/%.cc=obj/%.o}}

.PHONY: all clean

.PRECIOUS: ${OBJS}


all:  zuse ${PRFN}

zuse: CXXFLAGS := ${CXXFLAGS} -O2 -fno-reorder-blocks-and-partition -fno-reorder-functions -fipa-pta -fno-plt -fno-semantic-interposition -flto=auto -fdevirtualize-at-ltrans -floop-nest-optimize -fgraphite-identity
zuse: LDFLAGS  := -Wl,-s,-O1,--sort-common,-Bsymbolic,-z,relro,-z,combreloc ${LDFLAGS}
zuse: bin/zuse

bin/%:  obj/%.o ${OBJS}
	${LD} ${LDFLAGS} $^ -o $@

obj/%.o: src/%.cc
	${shell [[ ! -d ${@D} ]] && mkdir -p ${@D}}
	${CXX} ${CXXFLAGS} -c $^ -o $@



prf/%: CXXFLAGS := ${CXXFLAGS} -Og -ggdb
prf/%: LDFLAGS  := -Wl,-O1 ${LDFLAGS}
prf/%: ${OBJS} prf/%.o obj/gerat/prufung.o
	${LD} ${LDFLAGS} $^ -o $@



clean:
	${RM} ${wildcard bin/*} ${wildcard obj/*} ${PRFN}
