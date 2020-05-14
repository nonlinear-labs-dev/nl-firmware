#!/bin/sh

BINARY_DIR=$(realpath $1)
shift
SOURCE_DIR=$(realpath $1)
shift
DOCKER_ARGS="$1"
shift

TMPSCRIPT=$(mktemp)
TMPSCRIPTBASE=$(basename $TMPSCRIPT)
TMPDIRNAME="$BINARY_DIR/tmp"
TMPSCRIPT="$TMPDIRNAME/$TMPSCRIPTBASE"
mkdir -p $TMPDIRNAME

echo "" > $TMPSCRIPT

for var in "$@"
do
    echo $var >> $TMPSCRIPT
done

DOCKERNAME="nl-cross-build-environment"
docker run $DOCKER_ARGS --privileged --rm -v $TMPDIRNAME:/script -v $BINARY_DIR:/workdir -v $SOURCE_DIR:/sources $DOCKERNAME bash /script/$TMPSCRIPTBASE
