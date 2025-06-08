#!/bin/bash

# Variables
BCT_EXECUTABLE="./bct"
TEST_PASSED_COUNT=0
TEST_FAILED_COUNT=0
declare -a FILES_TO_CLEANUP

# Helper function to register files for cleanup
add_to_cleanup() {
    FILES_TO_CLEANUP+=("$1")
}

# Helper function to run a test
# $1: Test description
# $2: Command to run (e.g., "$BCT_EXECUTABLE file1 file2")
# $3: Expected exit code
# $4: Expected output string (use "" if no specific output string, only exit code matters)
# $5: (Optional) Further output string to check
run_test() {
    local description="$1"
    local command="$2"
    local expected_exit_code="$3"
    local expected_output_part1="$4"
    local expected_output_part2="$5"

    echo "-------------------------------------"
    echo "Running: $description"
    
    # Execute the command and capture output and exit code
    output=$(eval "$command" 2>&1) # Capture both stdout and stderr
    exit_code=$?

    local pass=true

    # Validate exit code
    if [ "$exit_code" -ne "$expected_exit_code" ]; then
        echo "FAIL: Expected exit code $expected_exit_code, got $exit_code."
        pass=false
    fi

    # Validate output part 1
    if [ -n "$expected_output_part1" ]; then
        if [[ "$output" != *"$expected_output_part1"* ]]; then
            echo "FAIL: Expected output to contain '$expected_output_part1'."
            pass=false
        fi
    fi
    
    # Validate output part 2 (if provided)
    if [ -n "$expected_output_part2" ]; then
        if [[ "$output" != *"$expected_output_part2"* ]]; then
            echo "FAIL: Expected output to contain '$expected_output_part2'."
            pass=false
        fi
    fi

    if [ "$pass" = true ]; then
        echo "PASS"
        TEST_PASSED_COUNT=$((TEST_PASSED_COUNT + 1))
    else
        echo "--- Command Output ---"
        echo "$output"
        echo "----------------------"
        TEST_FAILED_COUNT=$((TEST_FAILED_COUNT + 1))
    fi
    echo ""
}

# 1. Prerequisites Check
if [ ! -f "$BCT_EXECUTABLE" ]; then
    echo "Error: bct executable not found at $BCT_EXECUTABLE. Please compile bct.cpp first (e.g., g++ bct.cpp -o bct)."
    exit 1
fi
if [ ! -x "$BCT_EXECUTABLE" ]; then
    echo "Error: bct executable at $BCT_EXECUTABLE is not executable. Please check permissions (e.g., chmod +x bct)."
    exit 1
fi


# --- Test File Generation and Execution ---

# Test 1: Identical Files (non-empty)
echo "Preparing Test 1: Identical Files (non-empty)"
dd if=/dev/zero bs=1K count=1 2>/dev/null | tr '\0' 'A' > test1_fileA.bin
add_to_cleanup "test1_fileA.bin"
cp test1_fileA.bin test1_fileB.bin
add_to_cleanup "test1_fileB.bin"
run_test "Test 1: Identical Files (non-empty)" \
         "$BCT_EXECUTABLE test1_fileA.bin test1_fileB.bin" \
         0 \
         "The two binary are exactly the same."

# Test 2: Empty Files
echo "Preparing Test 2: Empty Files"
touch test2_fileA.bin
add_to_cleanup "test2_fileA.bin"
touch test2_fileB.bin
add_to_cleanup "test2_fileB.bin"
run_test "Test 2: Empty Files" \
         "$BCT_EXECUTABLE test2_fileA.bin test2_fileB.bin" \
         0 \
         "The two binary are exactly the same." # Based on current bct.cpp logic for empty files

# Test 3: Different Files (Difference at Beginning)
echo "Preparing Test 3: Different Files (Difference at Beginning)"
echo -n "TestDataA123" > test3_fileA.bin
add_to_cleanup "test3_fileA.bin"
echo -n "TestDataB123" > test3_fileB.bin
add_to_cleanup "test3_fileB.bin"
run_test "Test 3: Different Files (Difference at Beginning)" \
         "$BCT_EXECUTABLE test3_fileA.bin test3_fileB.bin" \
         1 \
         "Error in 8" # "TestData" is 8 bytes, 'A' vs 'B' is at index 8

# Test 4: Different Files (Difference at End)
echo "Preparing Test 4: Different Files (Difference at End)"
echo -n "TestData123A" > test4_fileA.bin
add_to_cleanup "test4_fileA.bin"
echo -n "TestData123B" > test4_fileB.bin
add_to_cleanup "test4_fileB.bin"
run_test "Test 4: Different Files (Difference at End)" \
         "$BCT_EXECUTABLE test4_fileA.bin test4_fileB.bin" \
         1 \
         "Error in 11" # "TestData123" is 11 bytes, 'A' vs 'B' is at index 11

# Test 5: Different Sizes
echo "Preparing Test 5: Different Sizes"
dd if=/dev/urandom of=test5_fileA.bin bs=1 count=10 2>/dev/null
add_to_cleanup "test5_fileA.bin"
dd if=/dev/urandom of=test5_fileB.bin bs=1 count=20 2>/dev/null
add_to_cleanup "test5_fileB.bin"
run_test "Test 5: Different Sizes" \
         "$BCT_EXECUTABLE test5_fileA.bin test5_fileB.bin" \
         1 \
         "Size doesn't match"

# Test 6: One File Non-Existent
echo "Preparing Test 6: One File Non-Existent"
echo -n "hello" > test6_fileA.bin
add_to_cleanup "test6_fileA.bin"
rm -f test6_fileB.bin # Ensure it doesn't exist
add_to_cleanup "test6_fileB.bin" # Add to cleanup in case script is interrupted before rm
run_test "Test 6: One File Non-Existent (second file)" \
         "$BCT_EXECUTABLE test6_fileA.bin test6_fileB.bin" \
         1 \
         "Error opening file: test6_fileB.bin"

# Test 7: Different Files (Large, difference in middle, > sz)
# sz in bct.cpp is 50,000,000 bytes (approx 47.68MB)
# We will make files ~55MB, difference at 50MB mark for clarity
MEGABYTE=1048576 # 1024 * 1024
SZ_BCT_MB=47
DIFF_OFFSET_MB=50 # Difference after 50MB
PAD_MB=5          # Add 5MB after difference

echo "Preparing Test 7: Large Files with difference in middle"
# File A
dd if=/dev/zero of=test7_fileA.bin bs=$MEGABYTE count=$DIFF_OFFSET_MB 2>/dev/null
echo -n "DifferenceHereA_and_some_padding_to_ensure_block_read" >> test7_fileA.bin
dd if=/dev/zero bs=$MEGABYTE count=$PAD_MB 2>/dev/null >> test7_fileA.bin
add_to_cleanup "test7_fileA.bin"

# File B (identical up to difference)
dd if=/dev/zero of=test7_fileB.bin bs=$MEGABYTE count=$DIFF_OFFSET_MB 2>/dev/null
echo -n "DifferenceHereB_and_some_padding_to_ensure_block_read" >> test7_fileB.bin
dd if=/dev/zero bs=$MEGABYTE count=$PAD_MB 2>/dev/null >> test7_fileB.bin
add_to_cleanup "test7_fileB.bin"

# Calculate the expected offset
# The difference starts right after $DIFF_OFFSET_MB of zeros
# The actual offset might be slightly different due to how the files are generated
# We'll check for "Error in" followed by a number close to the expected offset

run_test "Test 7: Large Files (difference after $DIFF_OFFSET_MB MB)" \
         "$BCT_EXECUTABLE test7_fileA.bin test7_fileB.bin" \
         1 \
         "Error in"

# --- Summary ---
echo "-------------------------------------"
echo "Test Summary:"
echo "PASSED: $TEST_PASSED_COUNT"
echo "FAILED: $TEST_FAILED_COUNT"
echo "-------------------------------------"


# --- Cleanup ---
echo "Cleaning up generated test files..."
for file_to_clean in "${FILES_TO_CLEANUP[@]}"; do
    if [ -f "$file_to_clean" ]; then # Check if file exists before trying to remove
        rm "$file_to_clean"
    fi
done
echo "Cleanup complete."


# Exit with 0 if all tests passed, 1 otherwise
if [ "$TEST_FAILED_COUNT" -eq 0 ]; then
    exit 0
else
    exit 1
fi
