#!/bin/bash
src_path=./src/mylib
dst_path=./build/bin
list=$src_path/*.c
for str in $list
do
    target=${str/$src_path\//}
    target=${target%.*}
    gcc $str -o $dst_path/$target -lpthread -lm
done
exit