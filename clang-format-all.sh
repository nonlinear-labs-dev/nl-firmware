#!/bin/bash

work_dir=$(git rev-parse --show-toplevel)
clang_format_file=/workdir/.clang-format
/bin/bash $work_dir/create_clang_format_docker.sh

echo $clang_format_file


clang_format_files=$(find $work_dir -iname .clang-format -printf '/workdir/%P\n')

echo $clang_format_files
exit 1

files=$(find $work_dir/projects/ -iname *.h -o -iname *.cpp -printf '/workdir/projects/%P\n')
echo $files
docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 clang-format-14 -style=file:$clang_format_file --verbose -i $files
