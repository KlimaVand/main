# We have 3 ways of compiling Fasset:
# - as normal standalone program: make Fasset
# - as standalone program with NitroScape changes: make Fasset NITROSCAPE=YES
# - as a libraray for NitroScape: make libnitro.a
AR=echo + $@ && ar -r
CC=echo + $@ && g++
CFLAGS=-c -Wall --coverage
LDFLAGS=-lgcov
SOURCES = NewMain.cpp
SOURCES += $(wildcard base/*.cpp)
SOURCES += $(wildcard building/*.cpp)
SOURCES += $(wildcard tools/*.cpp)
SOURCES += $(wildcard economy/*.cpp)
SOURCES += $(wildcard event/*.cpp)
SOURCES += $(wildcard fields/*.cpp)
SOURCES += $(wildcard Nitroscape/*.cpp)
SOURCES += $(wildcard fields/crops/*.cpp)
SOURCES += $(wildcard fields/fieldOperation/*.cpp)
SOURCES += $(wildcard fields/staticCrops/*.cpp)
SOURCES += $(wildcard legislation/*.cpp)
SOURCES += $(wildcard livestock/*.cpp)
SOURCES += $(wildcard manager/*.cpp)
SOURCES += $(wildcard products/*.cpp)
SOURCES += $(wildcard soil/*.cpp)
SOURCES += $(wildcard technics/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Fasset
# call make libnitro.a
LIB=libnitro.a
LIBSOURCES:=$(filter-out Nitroscape/FARM.cpp,$(SOURCES))
LIBSOURCES:=$(filter-out NewMain.cpp,$(SOURCES))
LIBOBJECTS=$(LIBSOURCES:.cpp=.o)
$(LIB):	CC=echo + $@ && mpic++
$(LIB): CFLAGS=-c -Wall -DNITROSCAPE
$(LIB): LDFLAGS=
# call make Fasset NITROSCAPE=YES
ifeq ($(NITROSCAPE), YES)
	CFLAGS+=-DNITROSCAPE
endif

prefix=/usr/bin
manpage=/usr/share/man/man1

.SILENT:

all: $(SOURCES) $(EXECUTABLE)

install: Fasset
	-mkdir -p ${DESTDIR}$(prefix)
	install -m 0755 Fasset ${DESTDIR}$(prefix)
	rm Fasset
	-mkdir -p ${DESTDIR}$(manpage)
	install -m 0755 Fasset.1.gz ${DESTDIR}$(manpage)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(LIB): $(LIBOBJECTS)
	$(AR) $@ $(LIBOBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -DUNIX -g -std=gnu++0x $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(LIB) $(OBJECTS) $(SOURCES:.cpp=.gcda) $(SOURCES:.cpp=.gcno)
