// Binary Compare Tool (bct)
// Compares two binary files byte-by-byte and reports the first difference or confirms they are identical.

#include <iostream>
#include <fstream>
#include <vector>   // For std::vector
#include <cstdio>   // For ferror, fclose, FILE, fopen, fread, etc. (though often included via iostream/fstream)
// It's good practice to include cstddef for size_t, but often available via other headers.

// Using namespace std; // It's often better to use std:: prefix explicitly.
#pragma warning(disable:4996) // For MSVC specific warnings like fopen_s

const __int64 sz = 50000000; // Buffer size

/**
 * @brief Compares two files byte-by-byte.
 * 
 * Reads files in chunks, compares them, and reports progress.
 * Handles file I/O errors and size mismatches.
 * 
 * @param s1_path Path to the first file.
 * @param s2_path Path to the second file.
 * @return true if files are identical, false otherwise (due to differences or errors).
 */
bool compare(char* s1_path, char* s2_path) {
	FILE *f1 = nullptr, *f2 = nullptr;

	f1 = fopen(s1_path, "rb");
	if (!f1) {
		std::cerr << "Error opening file: " << s1_path << std::endl;
		return false;
	}

	f2 = fopen(s2_path, "rb");
	if (!f2) {
		std::cerr << "Error opening file: " << s2_path << std::endl;
		fclose(f1); // f1 was opened
		return false;
	}

	// Using std::vector for RAII-style buffer management
	std::vector<unsigned char> buffer1_vec(sz);
	std::vector<unsigned char> buffer2_vec(sz);
	unsigned char* m1 = buffer1_vec.data();
	unsigned char* m2 = buffer2_vec.data();

	_fseeki64(f1, 0, SEEK_END);
	__int64 size1 = _ftelli64(f1);
	_fseeki64(f1, 0, SEEK_SET);

	_fseeki64(f2, 0, SEEK_END);
	__int64 size2 = _ftelli64(f2);
	_fseeki64(f2, 0, SEEK_SET);

	if (size1 != size2) {
		std::cout << "Size doesn't match" << std::endl; // As per original, message to cout
		fclose(f1);
		fclose(f2);
		return false;
	}

    if (size1 == 0) { // Both files are empty
        // std::cout << "Files are empty and thus identical." << std::endl; // Optional: message for empty files
        fclose(f1);
        fclose(f2);
        return true; // Empty files are considered identical
    }

	__int64 current_block_offset = 0; 

	while (current_block_offset < size1) {
		size_t bytes_read1 = fread(m1, sizeof(unsigned char), sz, f1);
		size_t bytes_read2 = fread(m2, sizeof(unsigned char), sz, f2);

		if (ferror(f1) || ferror(f2)) {
			std::cerr << "Error: File read error occurred after offset " << current_block_offset << std::endl;
			fclose(f1);
			fclose(f2);
			return false;
		}

		if (bytes_read1 != bytes_read2) {
			// This case should ideally not be reached if ferror is checked and sizes are initially equal,
			// unless there's an issue with the read itself that doesn't set ferror but returns different byte counts.
			std::cerr << "Error: Read size mismatch (f1:" << bytes_read1 << " vs f2:" << bytes_read2 
			          << ") for block starting at offset " << current_block_offset << std::endl;
			fclose(f1);
			fclose(f2);
			return false;
		}

		if (bytes_read1 == 0) {
			// This implies a clean EOF for both files if current_block_offset == size1.
			// If current_block_offset < size1, it's an unexpected EOF.
			if (current_block_offset < size1) {
                 std::cerr << "Error: Unexpected EOF encountered at offset " << current_block_offset 
                           << " when file size is " << size1 << std::endl;
				fclose(f1);
				fclose(f2);
				return false;
            }
			break; // Normal EOF, end of loop
		}

		for (long i = 0; i < bytes_read1; i++) {
			if (m1[i] != m2[i]) {
				std::cout << "Error in " << current_block_offset + i << std::endl; // As per original, message to cout
				fclose(f1);
				fclose(f2);
				return false; // Files differ
			}
		}
		
		current_block_offset = _ftelli64(f1); // Update offset based on where f1 pointer is
		
		if (size1 > 0) { // To avoid division by zero for empty files (though already handled)
			std::cout << "Progress: " << (static_cast<double>(current_block_offset) / size1) * 100.0 << "%"<< std::endl;
		}
	}

	// If the loop completes without returning false, the files are identical.
	fclose(f1);
	fclose(f2);
	return true;
}

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
