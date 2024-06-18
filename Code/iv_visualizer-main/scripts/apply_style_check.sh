#!/bin/bash

# Function to apply clang-format to all C++ files
apply_clang_format() {
  find . -name "*.cpp" -o -name "*.h" -exec clang-format -i {} \;
}

# Function to apply clang-tidy to all C++ files
apply_clang_tidy() {
  find . -name "*.cpp" -o -name "*.h" | xargs clang-tidy -fix
}

# Execute the functions
echo "Applying clang-format..."
apply_clang_format

echo "Applying clang-tidy..."
apply_clang_tidy

echo "Done."
