CXX = g++ -pg -std=gnu++11 -O3
CXXFLAGS = -Wall -g -D ROUTINE_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = Routine.o
PROGRAM = Routine.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
