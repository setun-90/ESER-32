# Commands
RM := rm -rf

# Flags
CXXFLAGS     := -I./include/ -Wall -Wextra -Wpedantic -march=native -std=c++11
LDFLAGS      := -ldl -pthread
ifeq (${MODE},gdb)
CXXFLAGS     := ${CXXFLAGS} -Og -ggdb
LDFLAGS      := -Wl,-O1 ${LDFLAGS}
else
CXXFLAGS     := ${CXXFLAGS} -Wa,-mbranches-within-32B-boundaries -O2 -fipa-pta -fno-plt -fno-semantic-interposition -flto=auto -fdevirtualize-at-ltrans -floop-nest-optimize -fgraphite-identity
LDFLAGS      := -Wl,-s,-O1,--sort-common,-z,now,-z,relro ${LDFLAGS}
endif
LD           := ${CXX}

# Items
TGT := zuse
TGT-SRC := src/zuse.cc
TGT-OBJ := obj/zuse.o
PRF-SRCS := ${wildcard prf/*.cc}
PRFN := ${PRF-SRCS:%.cc=%}
SRCS := ${wildcard src/*.cc}
OBJS := ${filter-out ${TGT-OBJ},${SRCS:src/%.cc=obj/%.o}}

${info ${shell [ -d bin ] || mkdir bin}}
${info ${shell [ -d obj ] || mkdir obj}}
${info ${shell [ -d lib ] || mkdir lib}}

.PHONY: all clean

.PRECIOUS: ${OBJS}

all:  zuse ${PRFN}

clean:
	${RM} ${wildcard bin/*} ${wildcard obj/*} ${PRFN}


zuse:      bin/zuse

bin/%: ${OBJS} obj/%.o
	${LD} ${LDFLAGS} $^ -o $@

obj/%.o:  src/%.cc
	${CXX} ${CXXFLAGS} -c $^ -o $@

prf/%: ${OBJS} prf/%.o
	${LD} ${LDFLAGS} $^ -o $@
