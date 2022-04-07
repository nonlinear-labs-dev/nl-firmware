FROM archlinux:base-20211010.0.36274

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring

RUN echo "Server = https://archive.archlinux.org/repos/2021/10/10/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist

RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib dosfstools archiso erofs-utils

COPY createIso.sh /
CMD /createIso.sh
