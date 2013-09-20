CXX = g++ -std=gnu++11
CXXFLAGS = -Wall -g -D_GLIBCXX_DEBUG -D CTRMODE_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = CtrMode.o
PROGRAM = CtrMode.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
