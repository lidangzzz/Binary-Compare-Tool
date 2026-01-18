## 2024-05-20 - Initial Optimization: `memcmp` for `bct`

**Learning:** The initial version of `bct` uses a byte-by-byte comparison loop, which is a major performance bottleneck for large files.

**Action:** Replace the loop with `memcmp` to leverage optimized, low-level memory comparison. Remove frequent progress reporting to minimize I/O overhead.
