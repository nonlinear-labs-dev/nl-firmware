#!/bin/bash

sourceDir="$1"
binaryDir="$2"
baseSourceDir="$3"

echo "$sourceDir"
echo "$binaryDir"
echo "$baseSourceDir"

docker run -v "$sourceDir":/sourceDir -v "$binaryDir":/binaryDir -v "$baseSourceDir":/source -it nl-configuration-build-container