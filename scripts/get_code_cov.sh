#!/bin/bash

cp debug/*.gcda .
cp debug/*.gcno .

for filename in $(find . | grep -E '\.cpp$');
do 
  echo "Filename: ${filename}"
  gcov -n -o . "${filename}" > /dev/null; 
done
