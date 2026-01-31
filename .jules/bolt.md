# Bolt's Journal - Binary Compare Tool

## 2025-05-14 - Compiler Optimization and memcmp
**Learning:** The project was missing compiler optimization flags in the Makefile and used a manual byte-by-byte comparison loop.
Adding `-O3` reduced execution time for 1GB comparison from ~4.8s to ~1.6s.
Replacing the manual loop with `std::memcmp` further reduced it to ~0.87s.
Combined, these optimizations achieved an ~82% reduction in execution time and a massive reduction in user CPU time (from 3.2s to 0.2s).
**Action:** Always check Makefile for optimization flags and use standard library functions like `memcmp` which are often highly optimized by the compiler or provided as specialized SIMD implementations.
