#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <cstring>

// Include the library header
#include "bct_lib.h"

// Helper function to create a test file with specific content
void create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create test file: " << filename << std::endl;
        exit(1);
    }
    file.write(content.c_str(), content.size());
    file.close();
}

// Helper function to clean up test files
void cleanup_test_files(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        std::remove(filename.c_str());
    }
}

// Test case: Compare identical files with special characters
void test_identical_files_with_special_chars() {
    std::string file1 = "test_special_1.bin";
    std::string file2 = "test_special_2.bin";
    std::vector<std::string> files_to_cleanup = {file1, file2};
    
    // Create test files with special characters
    std::string content = "Test\0Data\nWith\tSpecial\rChars";
    create_test_file(file1, content);
    create_test_file(file2, content);
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == true);
    
    std::cout << "Test identical_files_with_special_chars: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Test case: Compare files with difference in the middle
void test_difference_in_middle() {
    std::string file1 = "test_diff_middle_1.bin";
    std::string file2 = "test_diff_middle_2.bin";
    std::vector<std::string> files_to_cleanup = {file1, file2};
    
    // Create test files with a difference in the middle
    std::string content1 = "AAAABBBBCCCCDDDDEEEEFFFFGGGG";
    std::string content2 = "AAAABBBBCXCCDDDDEEEEFFFFGGGG";
    create_test_file(file1, content1);
    create_test_file(file2, content2);
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == false);
    
    std::cout << "Test difference_in_middle: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Test case: Compare with a non-existent file
void test_nonexistent_file() {
    std::string file1 = "test_exists.bin";
    std::string file2 = "test_nonexistent.bin";
    std::vector<std::string> files_to_cleanup = {file1};
    
    // Create only the first file
    std::string content = "This file exists";
    create_test_file(file1, content);
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == false);
    
    std::cout << "Test nonexistent_file: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Test case: Compare files with different sizes
void test_different_sizes() {
    std::string file1 = "test_size_1.bin";
    std::string file2 = "test_size_2.bin";
    std::vector<std::string> files_to_cleanup = {file1, file2};
    
    // Create test files with different sizes
    std::string content1 = "Short content";
    std::string content2 = "Longer content with more data";
    create_test_file(file1, content1);
    create_test_file(file2, content2);
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == false);
    
    std::cout << "Test different_sizes: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Test case: Compare empty files
void test_empty_files() {
    std::string file1 = "test_empty_1.bin";
    std::string file2 = "test_empty_2.bin";
    std::vector<std::string> files_to_cleanup = {file1, file2};
    
    // Create empty test files
    create_test_file(file1, "");
    create_test_file(file2, "");
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == true);
    
    std::cout << "Test empty_files: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Test case: Compare files with difference at the last byte
void test_difference_at_last_byte() {
    std::string file1 = "test_last_byte_1.bin";
    std::string file2 = "test_last_byte_2.bin";
    std::vector<std::string> files_to_cleanup = {file1, file2};
    
    // Create test files with a difference at the last byte
    std::string content1(1024, 'A'); // 1KB of 'A's
    std::string content2 = content1;
    content2[1023] = 'B'; // Change the last byte
    
    create_test_file(file1, content1);
    create_test_file(file2, content2);
    
    // Convert to char* for the compare function
    char file1_cstr[file1.size() + 1];
    char file2_cstr[file2.size() + 1];
    strcpy(file1_cstr, file1.c_str());
    strcpy(file2_cstr, file2.c_str());
    
    // Test the compare function
    bool result = compare(file1_cstr, file2_cstr);
    assert(result == false);
    
    std::cout << "Test difference_at_last_byte: PASSED" << std::endl;
    
    // Clean up
    cleanup_test_files(files_to_cleanup);
}

// Main function to run all tests
int main() {
    std::cout << "Running unit tests for Binary Compare Tool..." << std::endl;
    
    test_identical_files_with_special_chars();
    test_difference_in_middle();
    test_nonexistent_file();
    test_different_sizes();
    test_empty_files();
    test_difference_at_last_byte();
    
    std::cout << "All tests PASSED!" << std::endl;
    return 0;
}