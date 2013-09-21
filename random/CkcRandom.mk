CXX = g++ -std=gnu++11
CXXFLAGS = -Wall -g -D CKCRANDOM_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = CkcRandom.o
PROGRAM = CkcRandom.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
