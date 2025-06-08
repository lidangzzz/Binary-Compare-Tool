// Binary Compare Tool (bct) - Main Program
// Compares two binary files byte-by-byte and reports the first difference or confirms they are identical.

#include "bct.h"
#include <iostream>

/**
 * @brief Main entry point for the Binary Compare Tool.
 * 
 * Parses command-line arguments (expecting two file paths),
 * calls the compare function, and sets the exit code based on the result.
 * 
 * @param nNumberofArgs Number of command-line arguments.
 * @param pszArgs Array of command-line argument strings.
 * @return 0 if files are identical, 1 if they differ or an error occurs.
 */
int main(int nNumberofArgs, char* pszArgs[]) {
	if (nNumberofArgs != 3) {
		std::cout << "Usage: bct <file1> <file2>" << std::endl;
		return 1; // Indicate error
	}

	bool are_identical = compare(pszArgs[1], pszArgs[2]);

	if (are_identical) {
		std::cout << "The two binary are exactly the same." << std::endl;
		return 0; // Success
	} else {
		// The 'compare' function already prints specific error messages (e.g., size mismatch, read error, difference location).
		// So, no additional message is strictly needed here, just a different exit code.
		return 1; // Indicate difference or error
	}
}