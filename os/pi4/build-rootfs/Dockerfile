# Build environment for pi4 rootfs creation

FROM archlinux:20200908

RUN pacman --noconfirm -Sy

COPY createRootFS.sh /
COPY boot /boot-files
COPY root /root-files
