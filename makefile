# Commands
RM := rm -rf

# Flags
ifeq (${MODE},gdb)
CXXFLAGS     := -I./include/ -Wall -Wextra -Wpedantic -Og -g -march=native -std=c++11
LDFLAGS      := -Wl,-O1 -ldl -pthread
else
CXXFLAGS     := -I./include/ -Wall -Wextra -Wpedantic -O3 -march=native -std=c++11
LDFLAGS      := -Wl,-s,-O1 -ldl -pthread
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
