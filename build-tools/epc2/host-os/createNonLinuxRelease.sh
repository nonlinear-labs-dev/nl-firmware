#!/bin/bash

DOCKERNAME="$1"
OUT_FILE=$(realpath $2)
OUT_DIR=$(dirname $2)
PACKAGES="$3"

if tty; then
    TTY_OPTION=" -ti "
fi

docker run $TTY_OPTION --privileged -v $OUT_DIR/pac-cache:/var/cache/pacman -v $OUT_DIR/pac-conf:/var/lib/pacman ${DOCKERNAME} bash -c "pacman --noconfirm -Syy && pacman --noconfirm -Sw ${PACKAGES}"
tar -C $OUT_DIR -czvf $OUT_FILE ./pac-conf ./pac-cache
