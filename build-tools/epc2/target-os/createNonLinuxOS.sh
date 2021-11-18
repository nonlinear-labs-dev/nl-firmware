#!/bin/bash

DOCKERNAME="$1"
OUT_DIR=$(realpath $2)
IN_DIR=$(realpath $3)
PACKAGES_DIR=$OUT_DIR/../../build-container
INSTALL_PACKAGES="$4"
BUILD_CONTAINER_VERSION="$5"

SCRIPT=$(cat $IN_DIR/createNonLinuxOSDockerCommands.sh)

if tty; then
    TTY_OPTION=" -ti "
fi

docker run $TTY_OPTION --privileged \
  --env BUILD_CONTAINER_VERSION="$BUILD_CONTAINER_VERSION" \
  --env INSTALL_PACKAGES="$INSTALL_PACKAGES" \
  -v $PACKAGES_DIR:/packages \
  -v $OUT_DIR:/out \
  -v $IN_DIR:/in ${DOCKERNAME} bash -c "$SCRIPT"
    