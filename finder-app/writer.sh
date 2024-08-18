#!/bin/bash

# validate the number of arguments
if [ $# -ne 2 ]; then
  echo "Error: provide arguments $1 - file_path & $2 - text_string"
  exit 1
fi

writefile=$1
writestr=$2

# Create the file path if it does not exist
mkdir -p "$(dirname "$writefile")"

# write to the file
echo "$writestr" > "$writefile"

# Check if the file was created successfully
if [ $? -ne 0 ]; then
  echo "Error: Could not write to $writefile."
  exit 1
fi

exit 0

