include Makefile.arch

SOURCES=$(wildcard *.cc) $(wildcard Tools/*.cc) $(wildcard Tools/btagsf/*.cc) $(wildcard Tools/jetcorr/PhysicsTools/NanoAODTools/src/*.cc)
OBJECTS=$(SOURCES:.cc=.o)
CMSSWLIBS=-L${CMSSW_BASE}/lib/${SCRAM_ARCH} -lCondFormatsJetMETObjects -lJetMETCorrectionsModules
LIB=NANO_CORE.so

$(LIB): $(OBJECTS) 
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS) $(ROOTLIBS) -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer $(CMSSWLIBS) -o $@
	ln -sf $(LIB) lib$(LIB)

%.o:	%.cc
	# RHEL 7
	$(CXX) $(CXXFLAGS) -I${CMSSW_BASE}/../../../external/boost/1.67.0/include -I${CMSSW_BASE}/src -c $< -o $@ -fno-var-tracking
	# RHEL 8
	# $(CXX) $(CXXFLAGS) -I${CMSSW_BASE}/../../../external/boost/1.80.0-7f4aeae1bffcf24aa4723f09435633c2/include -I${CMSSW_BASE}/src -c $< -o $@ -fno-var-tracking

test: all
	python Tools/unit_tests/tests.py

testpy3: all
	python3 Tools/unit_tests/tests.py

all: $(LIB) 
clean:
	rm -f *.o \
	rm -f *.d \
	rm -f *.so \
	rm -f Tools/*.o \
	rm -f Tools/*.d \
	rm -f Tools/*.so \
	rm -f Tools/btagsf/*.o \
	rm -f Tools/btagsf/*.d \
	rm -f Tools/btagsf/*.so \
	rm -f Tools/jetcorr/PhysicsTools/NanoAODTools/src/*.o \
	rm -f Tools/jetcorr/PhysicsTools/NanoAODTools/src/*.d \
	rm -f Tools/jetcorr/PhysicsTools/NanoAODTools/src/*.so
