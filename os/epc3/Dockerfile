FROM archlinux:20200205

RUN pacman-key --init
RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman-key --populate

RUN echo "Server = https://archive.archlinux.org/repos/2021/10/10/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist
RUN sed -i 's/^SigLevel.*/SigLevel = Optional TrustAll/g' /etc/pacman.conf

RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib dosfstools archiso erofs-utils 
RUN pacman --noconfirm -S sudo binutils
