#!/bin/bash

sourceDir="$1"
binaryDir="$2"


podman run -v "$sourceDir":/sourcedir -v "$binaryDir":/binaryDir nl-configuration-build-container
