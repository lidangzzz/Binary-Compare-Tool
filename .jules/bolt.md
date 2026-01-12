# Bolt's Journal - Critical Learnings

This journal is for capturing critical, codebase-specific performance learnings.

## 2024-05-24 - C++ Standard vs. GTest Dependency

**Learning:** The project's Makefile was configured to use the C++11 standard (`-std=c++11`), but the installed version of Google Test requires C++14 or newer to compile. This created a deadlock where the tests could not be run with the project's original C++ standard.

**Action:** When working on this project, always use the C++14 standard to ensure that the tests can be compiled and run. The `Makefile` should be set to `-std=c++14`. This is a necessary environment change to ensure the project can be properly tested.
