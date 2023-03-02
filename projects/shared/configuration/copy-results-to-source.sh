#!/bin/bash

dstDir="$2/generated"
mkdir -p "${dstDir}"

cd "$1/generated/src/" || exit 2
cp  c15_config.h parameter_declarations.h parameter_list.h\
    parameter_descriptor.h placeholder.h parameter_group.h\
    display_scaling_type.h ../overview.html setting_list.h\
    $dstDir
