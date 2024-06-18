#!/bin/bash

 

set -e

 

echo ""

echo "Running Google C++ Style check and fix..."

 

# Fetch the commit SHA of the last commit.

last_commit_sha=$(git rev-parse HEAD)

previous_commit_sha=$(git rev-parse HEAD^)

 

if [ -z "${last_commit_sha}" ] || [ -z "${previous_commit_sha}" ]; then

    echo "Couldn't fetch the commit SHAs. Please ensure you have commits in your branch."

    exit 1

fi

 

# Check the formatting diff using clang-format-diff and redirect the output to format-diff.log.

git diff -U0 --no-color "${previous_commit_sha}" "${last_commit_sha}" | clang-format-diff -p1 > format-diff.log

 

# Check if there are any differences in the formatting and print the contents of format-diff.log if differences are found.

format_diff="$(<format-diff.log)"

if [ -n "${format_diff}" ]; then

    echo "Code formatting differences found:"

    cat format-diff.log

    exit 1

fi
