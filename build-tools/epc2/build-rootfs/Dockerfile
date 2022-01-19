# Build environment for arch rootfs creation

FROM archlinux:20200908

COPY createRootFS.sh /

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -Syy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib dosfstools

CMD /createRootFS.sh
