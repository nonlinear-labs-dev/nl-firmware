#!/bin/bash

work_dir=$(git rev-parse --show-toplevel)
/bin/bash $work_dir/create_clang_format_docker.sh
clang_format_files=$(find $work_dir -iname .clang-format -printf '%P\n')

for clang_format_file in $clang_format_files; do
	directory_name=$(dirname "${clang_format_file}")
	echo $directory_name
	in_docker_directory_name=/workdir/$directory_name
	files=$(find $directory_name \( -name '*.cpp' -o -name '*.h' -o -name '*.c' \) -a ! -path '*/cmake-build-*/*' -printf '%P\n')
	echo ${#files[@]}
	#docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 -style=file:/workdir/$clang_format_file
done

exit 1

files=$(find $work_dir/projects/ -iname *.h -o -iname *.cpp -printf '/workdir/projects/%P\n')
echo $files
docker run --rm -v "$work_dir":/workdir -w /workdir clang-format:14 clang-format-14 -style=file:$clang_format_file --verbose -i $files
