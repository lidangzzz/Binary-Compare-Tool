CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lgtest -lgtest_main -pthread

all: bct bct_test

bct: bct_main.o bct_lib.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bct_test: bct_test.cpp bct_lib.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

bct_main.o: bct_main.cpp bct.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bct_lib.o: bct_lib.cpp bct.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f bct bct_test *.o

test: bct_test
	./bct_test

.PHONY: all clean test