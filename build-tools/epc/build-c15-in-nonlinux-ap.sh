#!/bin/sh

BINARY_DIR=$1
SOURCE_DIR=$2

USER_ID=$(id -u $USER)
DOCKER_GROUP_ID=$(getent group docker | cut -d: -f3)
DOCKERNAME="nl-epc-build-environment-$USER"

docker run --rm -v ${SOURCE_DIR}/build-tools/epc:/script -v $BINARY_DIR:/workdir -v $SOURCE_DIR:/sources $DOCKERNAME bash /script/build-c15-in-nonlinux-ap-docker.sh
