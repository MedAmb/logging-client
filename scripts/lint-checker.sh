#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Set the path to your clang-tidy config file
CONFIG_FILE="$SCRIPT_DIR/../.clang-tidy"

# Find all C++ and header files in the project
FILES=$(find $SCRIPT_DIR/.. -not -path "$SCRIPT_DIR/../build/*" -name '*.cpp' -or -name '*.hpp')

# Run clang-tidy on each file
for FILE in $FILES
do
    # Run clang-tidy for linting
    clang-tidy -config-file=$CONFIG_FILE "$FILE"
    if [ $? -ne 0 ]; then
        echo "Error: $FILE has linting issues."
        exit 1
    fi
done

# If the script reaches this point, all files pass linting
echo "All files pass linting."
exit 0
