#!/bin/bash

echo "$@"

DOCKERNAME="$1"
OUT_FILE=$(basename $2)
OUT_DIR=$(dirname $2)
PACKAGES="$3"

echo "$@"

if tty; then
    TTY_OPTION=" -ti "
fi

SCRIPT=$(cat <<EOF
set -x
pacman --noconfirm -Syy
pacman --noconfirm -Sw ${PACKAGES}
tar -czvf /out/${OUT_FILE} ./pac-conf ./pac-cache)
EOF
)

if tty; then
    TTY_OPTION=" -ti "
fi

docker run $TTY_OPTION --privileged -v $OUT_DIR:/out -v $IN_DIR:/in ${DOCKERNAME} bash -c "$SCRIPT"
