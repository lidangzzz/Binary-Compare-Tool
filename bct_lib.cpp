// Binary Compare Tool (bct) - Library Implementation
// Compares two binary files byte-by-byte and reports the first difference or confirms they are identical.

#include "bct.h"
#include <iostream>
#include <fstream>
#include <vector>   // For std::vector
#include <cstdio>   // For ferror, fclose, FILE, fopen, fread, etc. (though often included via iostream/fstream)
#include <cstdint>  // For int64_t

#ifdef _MSC_VER
#pragma warning(disable:4996) // For MSVC specific warnings like fopen_s
#endif

const int64_t sz = 50000000; // Buffer size

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

	// Get file sizes using standard C functions
	fseek(f1, 0, SEEK_END);
	int64_t size1 = ftell(f1);
	fseek(f1, 0, SEEK_SET);

	fseek(f2, 0, SEEK_END);
	int64_t size2 = ftell(f2);
	fseek(f2, 0, SEEK_SET);

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

	int64_t current_block_offset = 0; 

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

		for (size_t i = 0; i < bytes_read1; i++) {
			if (m1[i] != m2[i]) {
				std::cout << "Error in " << current_block_offset + i << std::endl; // As per original, message to cout
				fclose(f1);
				fclose(f2);
				return false; // Files differ
			}
		}
		
		current_block_offset = ftell(f1); // Update offset based on where f1 pointer is
		
		if (size1 > 0) { // To avoid division by zero for empty files (though already handled)
			std::cout << "Progress: " << (static_cast<double>(current_block_offset) / size1) * 100.0 << "%"<< std::endl;
		}
	}

	// If the loop completes without returning false, the files are identical.
	fclose(f1);
	fclose(f2);
	return true;
}