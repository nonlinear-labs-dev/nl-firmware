FROM archlinux:20200908

ARG PACKAGES
RUN echo "Server = https://archive.archlinux.org/repos/2020/09/08/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist

RUN pacman -Sy
RUN pacman --noconfirm -S $PACKAGES


