CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS =

# Targets
all: bct unit_tests

bct: bct.cpp bct_lib.cpp bct_lib.h
	$(CXX) $(CXXFLAGS) bct.cpp bct_lib.cpp -o bct $(LDFLAGS)

unit_tests: unit_tests.cpp bct_lib.cpp bct_lib.h
	$(CXX) $(CXXFLAGS) unit_tests.cpp bct_lib.cpp -o unit_tests $(LDFLAGS)

# Run tests
test: bct unit_tests
	./run_tests.sh
	./unit_tests

# Run only unit tests
unit_test: unit_tests
	./unit_tests

# Clean up
clean:
	rm -f bct unit_tests *.o

.PHONY: all test unit_test clean