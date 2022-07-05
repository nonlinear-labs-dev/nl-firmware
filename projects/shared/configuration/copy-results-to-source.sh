#!/bin/bash

srcDir=$1
dstDir=$2

files=(c15_config.h parameter_declarations.h parameter_list.h parameter_descriptor.h placeholder.h parameter_group.h display_scaling_type.h)

numFiles=${#files[@]}
dstDir="${dstDir}/generated"
srcDir="${srcDir}/generated"

mkdir -p "${dstDir}"

for ((i=0; i<numFiles; i++)); do
    src=${files[${i}]}
    tgt=${files[${i}]}
    cp "${srcDir}/${src}" "${dstDir}/${tgt}"
done
