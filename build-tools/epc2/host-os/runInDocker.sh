#!/bin/sh

DOCKERNAME="$1"
shift
PACKAGES_DIR=$(realpath $1)
shift
BINARY_DIR=$(realpath $1)

shift

TMPSCRIPT=$(mktemp)
TMPSCRIPTBASE=$(basename $TMPSCRIPT)
TMPSCRIPTDIR=$(dirname $TMPSCRIPT)

echo "
set -e
set -x
" > $TMPSCRIPT

for var in "$@"
do
    echo $var >> $TMPSCRIPT
done

if tty; then
    TTY_OPTION=" -ti "
fi

docker run $TTY_OPTION --privileged -v $TMPSCRIPTDIR:/script -v $BINARY_DIR:/work -v $PACKAGES_DIR/pac-cache:/var/cache/pacman -v $PACKAGES_DIR/pac-conf:/var/lib/pacman ${DOCKERNAME} bash /script/$TMPSCRIPTBASE