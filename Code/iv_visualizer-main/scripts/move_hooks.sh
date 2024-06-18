#!/bin/bash

# Define the path to the Git hook file you want to install
HOOK_FILE="hooks/pre-commit"

# Check if the Git hook file exists
if [ -f "$HOOK_FILE" ]; then
    # Get the current Git repository's root directory
    REPO_ROOT=$(git rev-parse --show-toplevel)

    # Check if the current directory is a Git repository
    if [ -n "$REPO_ROOT" ]; then
        # Define the target directory for the hook
        HOOK_DIR="$REPO_ROOT/.git/hooks"

        # Move the hook file to the Git repository's hooks directory
        mv "$HOOK_FILE" "$HOOK_DIR"

        # Make the hook file executable
        chmod +x "$HOOK_DIR/$(basename "$HOOK_FILE")"

        echo "Git hook installed successfully in $HOOK_DIR."
    else
        echo "Error: This is not a Git repository."
        exit 1
    fi
else
    echo "Error: The specified Git hook file does not exist."
    exit 1
fi
