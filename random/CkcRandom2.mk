CXX = g++ -std=gnu++11
CXXFLAGS = -Wall -g -D CKCRANDOM2_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = CkcRandom2.o
PROGRAM = CkcRandom2.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
