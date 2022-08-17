include Makefile.arch

SOURCES_NOCMSSW=$(wildcard *.cc) $(wildcard Tools/*.cc) $(wildcard Tools/btagsf/*.cc)
SOURCES=$(SOURCES_NOCMSSW) $(wildcard Tools/jetcorr/PhysicsTools/NanoAODTools/src/*.cc)
OBJECTS_NOCMSSW=$(SOURCES_NOCMSSW:.cc=.o)
OBJECTS=$(SOURCES:.cc=.o)
LIB=NANO_CORE.so

$(LIB): $(OBJECTS) 
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS) $(ROOTLIBS) -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer -o $@
	ln -sf $(LIB) lib$(LIB)

%.o:	%.cc
	$(CXX) $(CXXFLAGS) -I${CMSSW_BASE}/../../../external/boost/1.67.0/include -I${CMSSW_BASE}/src -c $< -o $@ -fno-var-tracking

test: all
	python Tools/unit_tests/tests.py

testpy3: all
	python3 Tools/unit_tests/tests.py

all: $(LIB) 
nocmssw: $(OBJECTS_NOCMSSW) 
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS_NOCMSSW) $(ROOTLIBS) -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer -o $@
	ln -sf $(LIB) lib$(LIB)
clean:
	rm -f *.o \
	rm -f *.d \
	rm -f *.so \
	rm -f Tools/*.o \
	rm -f Tools/*.d \
	rm -f Tools/*.so
