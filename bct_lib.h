// Binary Compare Tool Library Header (bct_lib.h)
#ifndef BCT_LIB_H
#define BCT_LIB_H

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
bool compare(char* s1_path, char* s2_path);

#endif // BCT_LIB_H