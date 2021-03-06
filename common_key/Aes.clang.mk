CXX = clang++ -std=gnu++11
CXXFLAGS = -Wall -g -D __STRICT_ANSI__ -D AES_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = Aes.o
PROGRAM = Aes.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
