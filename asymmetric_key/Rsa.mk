CXX = g++ -std=gnu++11 -O3
CXXFLAGS = -Wall -pg -g -D RSA_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = Rsa.o
PROGRAM = Rsa.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
