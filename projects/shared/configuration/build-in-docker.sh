#!/bin/bash

sourceDir="$1"
binaryDir="$2"
isDevPC="$3"
container="nl-configuration-build-container"

if [ "$isDevPC" == "On" ] || [ "$isDevPC" == "ON" ] || [ "$isDevPC" == "on" ]
then
  echo "building configuration in podman (Development PC)"
  podman build -t $container $1
  podman run -v "$sourceDir":/sourceDir -v "$binaryDir":/binaryDir $container /sourceDir/process.sh /sourceDir /binaryDir
else
  echo "building configuration with native tools (Build Update)"
  /bin/bash "$sourceDir"/process.sh "$sourceDir" "$binaryDir"
fi

