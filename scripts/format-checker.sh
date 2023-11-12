#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Set the path to clang-format style file
STYLE_FILE="$SCRIPT_DIR/../.clang-format"

# Find all C++ and header files in the project
FILES=$(find $SCRIPT_DIR/.. -not -path "$SCRIPT_DIR/../build/*" -name '*.cpp' -or -name '*.hpp')

error_occurred=0

for FILE in $FILES
do
    # Run clang-format and store the formatted output in a variable
    FORMATTED_OUTPUT=$(clang-format -style=file "$FILE")

    # Use diff to compare the original file with the formatted output
    DIFF=$(diff -u "$FILE" <(echo "$FORMATTED_OUTPUT"))

    # If there are differences, print filename
    if [ -n "$DIFF" ]; then
        error_occurred=1
        echo "Error: $FILE has formatting issues."
    fi
done

# If an error occurred, exit with a non-zero status
if [ $error_occurred -eq 1 ]; then
    exit 1
fi

# If the script reaches this point, all files have correct formatting
echo "All files have correct formatting."
exit 0
