FROM archlinux:base-20220424.0.54084

RUN pacman-key --init
RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman-key --populate

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -S pacman binutils arch-install-scripts fakeroot pacman-contrib dosfstools archiso erofs-utils efibootmgr linux mkinitcpio mkinitcpio-archiso nano syslinux
