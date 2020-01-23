#!/bin/sh

BINARY_DIR=$1
shift
SOURCE_DIR=$1
shift

SCRIPT="echo \"Starting script:\""

for var in "$@"
do
        SCRIPT="$SCRIPT && $var"
done

SCRIPT="$SCRIPT && echo \"Script done.\""

USER_ID=$(id -u $USER)
DOCKER_GROUP_ID=$(getent group docker | cut -d: -f3)
DOCKERNAME="nl-cross-build-environment-$USER"

docker run -u $USER_ID:$DOCKER_GROUP_ID --rm -v $BINARY_DIR:/workdir -v $SOURCE_DIR:/sources $DOCKERNAME bash -c "$COMMANDS"
