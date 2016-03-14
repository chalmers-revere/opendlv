#!/bin/bash

for file in `find . -name \*.cpp -o -name \*.hpp`; do 
  clang-format $file > /tmp/formatted_file;
  cat /tmp/formatted_file > $file;
done

rm /tmp/formatted_file;
