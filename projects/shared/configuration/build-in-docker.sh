#!/bin/bash

sourceDir="$1"
binaryDir="$2"
container="nl-configuration-build-container"

podman build -t $container $1
podman run -v "$sourceDir":/sourceDir -v "$binaryDir":/binaryDir $container /sourceDir/process.sh /sourceDir /binaryDir
