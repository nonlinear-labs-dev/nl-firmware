#!/bin/bash

work_dir=$(git rev-parse --show-toplevel)

/bin/bash $work_dir/hooks/create_clang_format_container.sh $work_dir

all_files=$(find -iname *.h -o -iname *.cpp -o -iname *.c -o -iname *.hpp | grep -v 'cmake-build-' | wc -l)

in_container_paths=("${all_files[@]/#/'/workdir/'}")

podman run --rm -v $work_dir:/workdir -w /workdir clang-format:14 clang-format-14 -i $in_container_paths