CXX = g++ -pg -std=gnu++11
CXXFLAGS = -O3 -Wall -g -D BASICBIGNATURAL_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = BasicBigNatural.o
PROGRAM = BasicBigNatural.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
