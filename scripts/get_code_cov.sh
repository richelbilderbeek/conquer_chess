#!/bin/bash

echo "Copying gcov files to root folder"
cp debug/*.gcda .
cp debug/*.gcno .

echo "Extracting coverage info from the gcov files"
for filename in $(find . | grep -E '\.cpp$');
do 
  echo "Filename: ${filename}"
  gcov -n -o . "${filename}" > /dev/null
done

echo "Succes: done extracting code coverage information"
exit 0
