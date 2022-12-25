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
LDFLAGS      := -Wl,-O1,--as-needed,--sort-common,-z,relro,-z,combreloc
LDLIBS       := -ldl -pthread
# Recover compiler and link commands
LINK         = ${LINK.cc} $^ ${LOADLIBES} ${LDLIBS} -o $@
COMPILE      = ${COMPILE.cc} ${OUTPUT_OPTION} $<
MKDIR        = [ -d ${@D} ] || mkdir -p ${@D}

# Items
SRCS := ${shell find src/ prf/ \( -name '*.cc' -a \! -name zuse.cc \)}
GRTE := ${filter src/gerat/%,${SRCS}}
PRFN := ${filter prf/%,${SRCS}}
KERN := ${filter-out ${GRTE} ${PRFN},${SRCS}}
KERNEL = ${KERN:src/%.cc=${OUTPUT}/obj/%.o}



.PHONY: release debug tests clean



.SECONDEXPANSION:
# Release build
ifeq "${CXX}" "g++"
release: CXXFLAGS := ${CXXFLAGS} -O2 -fvisibility=hidden -fno-reorder-blocks-and-partition -fno-reorder-functions -fira-region=mixed -ftree-cselim -flive-range-shrinkage -fpredictive-commoning -ftree-loop-distribution -fsched-pressure -fweb -frename-registers -fipa-pta -flto=auto -flto-partition=one -floop-nest-optimize -fgraphite-identity -fno-plt -fno-semantic-interposition -fdevirtualize-at-ltrans -fasynchronous-unwind-tables
else ifeq "${CXX}" "clang++"
release: CXXFLAGS := ${CXXFLAGS} -O2 -flto=thin -fno-plt
endif
release: LDFLAGS  := ${LDFLAGS},-s
release: OUTPUT   := release
release: release/bin/zuse gerate
release/bin/%: release/obj/%.o $${KERNEL}
	${MKDIR}
	${LINK}
release/lib/%.so: LDFLAGS := ${LDFLAGS} -shared
release/lib/%.so: release/obj/gerat/%.o $${KERNEL}
	${MKDIR}
	${LINK}
release/obj/%.o: src/%.cc
	${MKDIR}
	${COMPILE}



# Debug build
debug: CXXFLAGS := ${CXXFLAGS} -Og -ggdb
debug: LDFLAGS  := ${LDFLAGS} -fsanitize=address
debug: OUTPUT   := debug
debug: debug/bin/zuse gerate
debug/bin/%: debug/obj/%.o $${KERNEL}
	${MKDIR}
	${LINK}
debug/lib/%.so: LDFLAGS := ${LDFLAGS} -shared
debug/lib/%.so: debug/obj/gerat/%.o $${KERNEL}
	${MKDIR}
	${LINK}
debug/obj/%.o: src/%.cc
	${MKDIR}
	${COMPILE}



# Test builds
tests: debug/lib/prufung.so ${PRFN:%.cc=%}
prf/%: CXXFLAGS := ${CXXFLAGS} -Og -ggdb
prf/%: LDFLAGS  := ${LDFLAGS} -fsanitize=address
prf/%: OUTPUT   := debug
prf/%: prf/%.o $${KERNEL}
	${LINK}
prf/durchgangeinheit: prf/durchgangeinheit.o debug/lib/prufung.so $${KERNEL}
prf/%.o: prf/%.cc



gerate: ${patsubst $${OUTPUT}/obj/gerat/%.o,$${OUTPUT}/lib/%.so,${GRTE:src/%.cc=$${OUTPUT}/obj/%.o}}



clean:
	${RM} debug release ${PRFN:%.cc=%}
