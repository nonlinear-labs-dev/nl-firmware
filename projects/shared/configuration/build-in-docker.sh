#!/bin/bash

sourceDir="$1"
binaryDir="$2"

podman build -t nl-configuration-build-container $1
podman run -v "$sourceDir":/sourceDir -v "$binaryDir":/binaryDir nl-configuration-build-container
