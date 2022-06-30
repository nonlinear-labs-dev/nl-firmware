#!/bin/bash

srcDir=$1
dstDir=$2

files=(config.h declarations.h definitions.h descriptor.h)
targetFiles=(c15_config.h parameter_declarations.h parameter_list.h parameter_descriptor.h)

numFiles=${#files[@]}
echo "${numFiles}"

ls "${srcDir}"

dstDir="${dstDir}/projects/shared/parameter-db/generated"
srcDir="${srcDir}/generated"

for ((i=0; i<numFiles; i++)); do
    src=${files[${i}]}
    tgt=${targetFiles[${i}]}
    echo "$srcDir/$src -> $dstDir/$tgt"
    cp "${srcDir}/${src}" "${dstDir}/${tgt}"
done
