#!/bin/bash

# validate the number of arguments
if [ $# -ne 2 ]; then
  echo "Error: Provide 2 arguments $1 - file's path & $2 search string."
  exit 1
fi

filesdir=$1
searchstr=$2

# validate the path 
if [ ! -d "$filesdir" ]; then
  echo "Error: $filesdir is not a valid directory."
  exit 1
fi

# count all files in the directory and its subdirectories
file_count=$(find "$filesdir" -type f | wc -l)

# Search for the text string in the files and count matching lines
match_count=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)

# Print the result
echo "The number of files are $file_count and the number of matching lines are $match_count"

exit 0

