CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES = NewMain.cpp
SOURCES += $(wildcard base/*.cpp)
SOURCES += $(wildcard building/*.cpp)
SOURCES += $(wildcard tools/*.cpp)
SOURCES += $(wildcard economy/*.cpp) 
SOURCES += $(wildcard event/*.cpp)
SOURCES += $(wildcard fields/*.cpp)
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

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -DUNIX  $< -o $@

clean:
	rm -rf *.o base/*.o building/*.o tools/*.o economy/*.o event/*.o fields/*.o fields/crops/*.o fields/fieldOperation/*.o fields/staticCrops/*.o legislation/*.o manager/*.o Nitroscape/*.o products/*.o soil/*.o technics/*.o
