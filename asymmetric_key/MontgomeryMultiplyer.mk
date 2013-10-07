CXX = g++ -std=gnu++11 -O3
CXXFLAGS = -Wall -g -D MONTGOMERYMULTIPLYER_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = MontgomeryMultiplyer.o
PROGRAM = MontgomeryMultiplyer.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
