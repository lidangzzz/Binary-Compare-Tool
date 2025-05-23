# Binary-Compare-Tool (bct)

A command-line tool to compare two binary files and identify the first point of difference. It's designed to be fast for large files.

## Compilation

You can compile the project using the provided Makefile:

```bash
make
```

Or manually using a C++ compiler like g++:

```bash
g++ bct.cpp bct_lib.cpp -o bct
```

Either way, this will produce an executable file named `bct`.

## Usage

To compare two binary files, run the tool from your terminal:

```bash
./bct <file1_path> <file2_path>
```

Replace `<file1_path>` and `<file2_path>` with the actual paths to the files you want to compare.

**Example:**

```bash
./bct myarchive.zip myarchive_corrupted.zip
```

## Output Messages

*   **"Must 2 files"**: Displayed if the tool is not run with exactly two file arguments.
*   **"Error opening file: [filename]"**: Displayed if one of the specified files cannot be opened for reading. Check if the file exists and you have read permissions.
*   **"Size doesn't match"**: Displayed if the two files have different sizes. The tool currently expects files of the same size for a full byte-by-byte comparison.
*   **"Error reading file: [filename]"**: Displayed if an error occurs while reading data from one of the files.
*   **"Error in [offset]"**: Displayed when a difference is found. `[offset]` is the byte offset (0-indexed) from the beginning of the file where the first mismatch occurs.
*   **"Progress: [percentage]%"**: Periodically displayed during the comparison of large files.
*   **"The two binary are exactly the same."**: Displayed if both files are identical in content and size.

## Exit Codes

*   **0**: Files are identical.
*   **1**: Files are different, or an error occurred (e.g., file not found, read error).

## Running Tests

There are two types of tests available:

### Integration Tests

A test script `run_tests.sh` is included to verify the functionality of `bct` with various file scenarios.
To run the integration tests:

1.  Ensure you have compiled the project and the `bct` executable is present in the root directory.
    ```bash
    make
    ```
2.  Make the test script executable (if it isn't already):
    ```bash
    chmod +x run_tests.sh
    ```
3.  Run the script:
    ```bash
    ./run_tests.sh
    ```
    
The script will output PASS or FAIL for each test case.

### Unit Tests

Unit tests are also available to test specific functionality of the comparison logic.
To run the unit tests:

1.  Compile the unit tests:
    ```bash
    make unit_tests
    ```
2.  Run the unit tests:
    ```bash
    ./unit_tests
    ```

### Running All Tests

You can run all tests at once using the Makefile:

```bash
make test
```

This will compile both the main program and unit tests, then run both test suites.

## Project Structure

*   `bct.cpp`: The main program entry point.
*   `bct_lib.cpp`: The core binary comparison logic.
*   `bct_lib.h`: Header file for the comparison library.
*   `run_tests.sh`: Integration test script to verify the functionality of the tool.
*   `unit_tests.cpp`: Unit tests for the comparison logic.
*   `run_unit_tests.sh`: Script to compile and run the unit tests.
*   `Makefile`: Build configuration for compiling and testing the project.
