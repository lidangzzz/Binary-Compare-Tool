#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdint>
#include "bct.h"

// Helper function to create a test file with specific content
void createTestFile(const std::string& filename, const std::vector<unsigned char>& content) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(content.data()), content.size());
        file.close();
    }
}

// Test fixture for Binary Compare Tool tests
class BCTTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files for each test
    }

    void TearDown() override {
        // Clean up test files
        std::remove("test_identical1.bin");
        std::remove("test_identical2.bin");
        std::remove("test_different1.bin");
        std::remove("test_different2.bin");
        std::remove("test_empty1.bin");
        std::remove("test_empty2.bin");
        std::remove("test_nonexistent.bin");
    }
};

// Test case for identical files
TEST_F(BCTTest, IdenticalFiles) {
    std::vector<unsigned char> content = {'A', 'B', 'C', 'D', 'E'};
    createTestFile("test_identical1.bin", content);
    createTestFile("test_identical2.bin", content);
    
    char file1[] = "test_identical1.bin";
    char file2[] = "test_identical2.bin";
    
    EXPECT_TRUE(compare(file1, file2));
}

// Test case for different files
TEST_F(BCTTest, DifferentFiles) {
    std::vector<unsigned char> content1 = {'A', 'B', 'C', 'D', 'E'};
    std::vector<unsigned char> content2 = {'A', 'B', 'X', 'D', 'E'};
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for empty files
TEST_F(BCTTest, EmptyFiles) {
    createTestFile("test_empty1.bin", {});
    createTestFile("test_empty2.bin", {});
    
    char file1[] = "test_empty1.bin";
    char file2[] = "test_empty2.bin";
    
    EXPECT_TRUE(compare(file1, file2));
}

// Test case for non-existent file
TEST_F(BCTTest, NonExistentFile) {
    std::vector<unsigned char> content = {'A', 'B', 'C', 'D', 'E'};
    createTestFile("test_identical1.bin", content);
    
    char file1[] = "test_identical1.bin";
    char file2[] = "test_nonexistent.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for different file sizes
TEST_F(BCTTest, DifferentFileSizes) {
    std::vector<unsigned char> content1 = {'A', 'B', 'C'};
    std::vector<unsigned char> content2 = {'A', 'B', 'C', 'D', 'E'};
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for difference at the beginning of the file
TEST_F(BCTTest, DifferenceAtBeginning) {
    std::vector<unsigned char> content1 = {'X', 'B', 'C', 'D', 'E'};
    std::vector<unsigned char> content2 = {'A', 'B', 'C', 'D', 'E'};
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for difference at the end of the file
TEST_F(BCTTest, DifferenceAtEnd) {
    std::vector<unsigned char> content1 = {'A', 'B', 'C', 'D', 'X'};
    std::vector<unsigned char> content2 = {'A', 'B', 'C', 'D', 'E'};
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for larger files (still small enough for quick testing)
TEST_F(BCTTest, LargerFiles) {
    // Create two identical files with 10KB of data
    std::vector<unsigned char> content1(10240, 'A');
    std::vector<unsigned char> content2(10240, 'A');
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_TRUE(compare(file1, file2));
}

// Test case for larger files with a difference
TEST_F(BCTTest, LargerFilesWithDifference) {
    // Create two files with 10KB of data, with a difference in the middle
    std::vector<unsigned char> content1(10240, 'A');
    std::vector<unsigned char> content2(10240, 'A');
    content1[5000] = 'X'; // Introduce a difference
    
    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);
    
    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";
    
    EXPECT_FALSE(compare(file1, file2));
}

// Test case for one empty and one non-empty file
TEST_F(BCTTest, OneEmptyAndOneNonEmptyFile) {
    createTestFile("test_empty1.bin", {});
    std::vector<unsigned char> content = {'A', 'B', 'C'};
    createTestFile("test_different1.bin", content);

    char file1[] = "test_empty1.bin";
    char file2[] = "test_different1.bin";

    EXPECT_FALSE(compare(file1, file2));
}

// Test case for files exactly the size of the buffer
TEST_F(BCTTest, FilesAtBufferSize) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content(bufferSize, 'A');
    createTestFile("test_different1.bin", content);
    createTestFile("test_different2.bin", content);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_TRUE(compare(file1, file2));
}

// Test case for files exactly the size of the buffer with a difference at the end
TEST_F(BCTTest, FilesAtBufferSizeWithDifference) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content1(bufferSize, 'A');
    std::vector<unsigned char> content2(bufferSize, 'A');
    content2[bufferSize - 1] = 'B'; // Difference at the very end

    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_FALSE(compare(file1, file2));
}

// Test case for files that are a multiple of the buffer size
TEST_F(BCTTest, FilesMultipleOfBufferSize) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content(bufferSize * 2, 'A');
    createTestFile("test_different1.bin", content);
    createTestFile("test_different2.bin", content);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_TRUE(compare(file1, file2));
}

// Test case for files that are a multiple of the buffer size with a difference
TEST_F(BCTTest, FilesMultipleOfBufferSizeWithDifference) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content1(bufferSize * 2, 'A');
    std::vector<unsigned char> content2(bufferSize * 2, 'A');
    content2[bufferSize + 100] = 'B'; // Difference in the second chunk

    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_FALSE(compare(file1, file2));
}

// Test case for files larger than the buffer size
TEST_F(BCTTest, FilesLargerThanBufferSize) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content(bufferSize + 1024, 'A');
    createTestFile("test_different1.bin", content);
    createTestFile("test_different2.bin", content);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_TRUE(compare(file1, file2));
}

// Test case for files larger than the buffer size with a difference
TEST_F(BCTTest, FilesLargerThanBufferSizeWithDifference) {
    const int64_t bufferSize = 50000000;
    std::vector<unsigned char> content1(bufferSize + 1024, 'A');
    std::vector<unsigned char> content2(bufferSize + 1024, 'A');
    content2[bufferSize + 500] = 'B'; // Difference in the second chunk

    createTestFile("test_different1.bin", content1);
    createTestFile("test_different2.bin", content2);

    char file1[] = "test_different1.bin";
    char file2[] = "test_different2.bin";

    EXPECT_FALSE(compare(file1, file2));
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}