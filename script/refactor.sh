#!/bin/bash

for file in `find code/system -name \*.cpp -o -name \*.hpp`; do 
  clang-format $file > /tmp/formatted_file;
  cat /tmp/formatted_file > $file;
done

for file in `find code/tools -name \*.cpp -o -name \*.hpp`; do 
  clang-format $file > /tmp/formatted_file;
  cat /tmp/formatted_file > $file;
done

rm /tmp/formatted_file;
