#!/bin/bash

# Compile the unit tests
echo "Compiling unit tests..."
g++ -c bct_lib.cpp -o bct_lib.o
g++ unit_tests.cpp bct_lib.o -o unit_tests

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Make the unit tests executable
chmod +x unit_tests

# Run the unit tests
echo "Running unit tests..."
./unit_tests

# Check if tests passed
if [ $? -ne 0 ]; then
    echo "Unit tests failed!"
    exit 1
else
    echo "All unit tests passed!"
fi

# Clean up
rm -f unit_tests bct_lib.o

exit 0