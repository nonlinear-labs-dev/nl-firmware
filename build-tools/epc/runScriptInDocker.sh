#!/bin/sh

BINARY_DIR=$(realpath $1)
SOURCE_DIR=$(realpath $2)
SCRIPT=$(cat $3)

DOCKERNAME="nl-epc-build-environment"

if tty; then
    TTY_OPTION=" -ti "
fi

GROUP_ID=$(id -g $USER)
USER_ID=$(id -u $USER)

docker run -ti $TTY_OPTION \
        -e GROUP_ID=$GROUP_ID -e USER_ID=$USER_ID \
        --privileged --rm -v $BINARY_DIR:/bindir -v $SOURCE_DIR:/sources $DOCKERNAME bash -c "$SCRIPT"
