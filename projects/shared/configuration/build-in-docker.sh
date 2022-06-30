#!/bin/bash

sourceDir="$1"
binaryDir="$2"
userID="$4"
groupID="$5"

echo "$sourceDir"
echo "$binaryDir"
echo "$userID"
echo "$groupID"

podman run -v "$sourceDir":/sourceDir -v "$binaryDir":/binaryDir -ti --privileged -e USER_ID="${userID}" -e GROUP_ID="${groupID}" nl-configuration-build-container