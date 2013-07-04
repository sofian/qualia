.PHONY: clean all bin install uninstall

CXX=g++
CXXFLAGS=-O3 -fpic
#CXXFLAGS=-O3 -fpic -Werror -Wall -Wextra -Weffc++ -pedantic-errors -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Wsign-promo
DEBUGFLAGS=-O0 -g -fpic

SRC :=
DIRS :=
include src/robgp/robgp.mk
OBJECTS=$(patsubst src/%,obj/%,$(SRC:.cpp=.o))
HEADERS=$(wildcard $(DIRS:%/=%/*.hpp))
OBJDIRS=$(DIRS:src/%=obj/%)
BINDIRS=$(DIRS:src/%=bin/%)

all: bin

obj/%.o: src/%.cpp src/%.hpp
	${CXX} ${CXXFLAGS} -c src/$*.cpp -o $@

obj/%/:
	mkdir -p $@

bin: bin/librobgp.so bin/robgp.a bin/robgp/robgp.hpp

bin/%/:
	mkdir -p $@

bin/librobgp.so: ${OBJDIRS} ${OBJECTS} ${BINDIRS}
	${CXX} -shared -Wl,-soname,librobgp.so -o $@ ${OBJECTS}

bin/robgp.a: ${OBJDIRS} ${OBJECTS} ${BINDIRS}
	ar rcs $@ ${OBJECTS}

bin/robgp/robgp.hpp: ${BINDIRS} src/robgp/header.txt ${HEADERS}
	cat src/robgp/header.txt > $@
	for i in $(patsubst src/robgp/%.hpp,%.hpp,${HEADERS}); do echo "#include \"$$i\"" >> $@; cp src/robgp/$$i bin/robgp/$$i; done

install: bin
	mkdir -p /usr/local/include/robgp
	mkdir -p /usr/local/lib
	cp -rv bin/robgp/* /usr/local/include/robgp/
	cp bin/librobgp.so /usr/local/lib/

uninstall:
	rm -rf /usr/local/include/robgp
	rm -f /usr/local/lib/librobgp.so

clean:
	rm -f ${OBJECTS} ${OBJDIRS}


