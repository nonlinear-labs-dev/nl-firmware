#!/bin/bash

srcDir=$1
dstDir=$2

files=(c15_config.h parameter_declarations.h parameter_list.h parameter_descriptor.h placeholder.h)
targetFiles=(c15_config.h parameter_declarations.h parameter_list.h parameter_descriptor.h placeholder.h)

numFiles=${#files[@]}
echo "${numFiles}"

ls "${srcDir}"

dstDir="${dstDir}/generated"
srcDir="${srcDir}/generated"

mkdir -p "${dstDir}"

for ((i=0; i<numFiles; i++)); do
    src=${files[${i}]}
    tgt=${targetFiles[${i}]}
    echo "$srcDir/$src -> $dstDir/$tgt"
    cp "${srcDir}/${src}" "${dstDir}/${tgt}"
done
