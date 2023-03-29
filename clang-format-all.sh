#!/bin/bash

work_dir=$(git rev-parse --show-toplevel)
clang_format_file=/workdir/.clang-format
/bin/bash $work_dir/create_clang_format_docker.sh

echo $clang_format_file

files=$(find $work_dir/projects/ -iname *.h -o -iname *.cpp -printf '/workdir/projects/%P\n')
echo $files
docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 clang-format-14 -style=file:$clang_format_file --verbose -i $files
exit 0

for file in $(find $work_dir/projects/ -iname *.h -o -iname *.cpp -printf '/workdir/projects/%P\n'); do
	docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 clang-format-14 -style=file:$clang_format_file -i $file
	echo $file
done
exit 1
| xargs docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 clang-format-14 -style=file:$clang_format_style -i
