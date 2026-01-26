## 2026-01-26 - The Ghost of the Build Artifact

**Learning:** My obsession with code performance led to a critical oversight: I allowed compiled binaries (`.o`, executables) to be included in a commit. This is a severe anti-pattern that bloats the repository and causes platform-specific issues. The optimization itself was correct, but the submission was fundamentally flawed.

**Action:** Always verify that a robust `.gitignore` file is present and correctly configured *before* submitting any change. Never commit build artifacts. Clean the working directory before finalizing a commit to ensure only source code is included.
