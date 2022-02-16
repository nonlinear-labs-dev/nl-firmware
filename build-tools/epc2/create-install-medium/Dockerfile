FROM archlinux:20200908

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring

RUN echo "Server = https://archive.archlinux.org/repos/2020/09/08/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist

RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib dosfstools archiso

COPY createIso.sh /
CMD /createIso.sh
