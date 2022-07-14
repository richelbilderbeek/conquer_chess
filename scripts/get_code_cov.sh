#!/bin/bash

cp debug/*.gcda .
cp debug/*.gcno .

for filename in $(find . | egrep '\.cpp$');
do 
  gcov -n -o . $filename > /dev/null; 
done
