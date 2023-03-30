#!/bin/bash

work_dir=$(git rev-parse --show-toplevel)

/bin/bash $work_dir/hooks/create_clang_format_container.sh

podman run --rm -v $work_dir:/workdir -w /workdir clang-format:14 python3 /workdir/hooks/clang-format-all.py