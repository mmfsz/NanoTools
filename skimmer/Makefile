# Simple makefile

EXE=skim

SOURCES=$(wildcard *.cc) $(wildcard ../rapido/src/*.cc) ../NanoCORE/Nano.cc  ../NanoCORE/ElectronSelections.cc ../NanoCORE/MuonSelections.cc ../NanoCORE/Config.cc ../NanoCORE/IsolationTools.cc
OBJECTS=$(SOURCES:.cc=.o)
HEADERS=$(SOURCES:.cc=.h)

CC          = g++
CXX         = g++
CXXFLAGS    = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LD          = g++
LDFLAGS     = -g -O2
SOFLAGS     = -g -shared
CXXFLAGS    = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LDFLAGS     = -g -O2
ROOTLIBS    = $(shell root-config --libs)
ROOTCFLAGS  = $(shell root-config --cflags)
CXXFLAGS   += $(ROOTCFLAGS)
CFLAGS      = $(ROOTCFLAGS) -Wall -Wno-unused-function -g -O2 -fPIC -fno-var-tracking -DLorentzVectorPtEtaPhiM4D
EXTRACFLAGS = -I../NanoCORE -I../rapido/src -I../NanoCORE/Tools -I${CMSSW_BASE}/../../../external/boost/1.67.0/include -I${CMSSW_BASE}/src
EXTRAFLAGS  = -fPIC -ITMultiDrawTreePlayer -Wunused-variable -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(ROOTLIBS) $(EXTRAFLAGS) -o $@

%.o: %.cc
	$(CC) $(CFLAGS) $(EXTRACFLAGS) $< -c -o $@

clean:
	rm -f main.o $(EXE)

cleanall:
	rm -f $(OBJECTS) $(EXE)
