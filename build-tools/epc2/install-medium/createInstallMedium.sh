#!/bin/bash

OUT_DIR=$(realpath $1)
IN_DIR=$(realpath $2)

if tty; then
    TTY_OPTION=" -ti "
fi

SCRIPT=$(cat <<EOF
pacman --noconfirm -Syy
pacman --noconfirm -S archiso
cp -r /usr/share/archiso/configs/baseline/ /archlive
cp $IN_DIR/bootfs.fat /archlive/airootfs/
cp $IN_DIR/rootfs.ext4 /archlive/airootfs/
gzip /archlive/airootfs/bootfs.fat
gzip /archlive/airootfs/rootfs.ext4
mkarchiso -v -w /work_dir -o /out /archlive
EOF
)

docker run $TTY_OPTION --privileged -v $OUT_DIR:/out -v $IN_DIR:/in archlinux:20200908 bash -c "$SCRIPT"