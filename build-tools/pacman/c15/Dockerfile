FROM archlinux:base-20220424.0.54084

ENV HOST_REQUIREMENTS arch-install-scripts binutils ccache cmake fakeroot gcc git linux make mc \
                      nano ninja pacman pacman-contrib pkgconfig sudo squashfs-tools
ENV C15_REQUIREMENTS  alsa alsa-tools alsa-utils base boost cpupower flac freetype2 glibmm libpng \
                      libsoup mc networkmanager png++ shared-mime-info sudo sshfs lsof systemd-sysvcompat

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -S $HOST_REQUIREMENTS
RUN pacman --noconfirm -S $C15_REQUIREMENTS

RUN mkdir -p /packages
RUN mkdir -p /tmp/blankdb
WORKDIR /packages
RUN pacman --noconfirm -Syw --cachedir . --dbpath /tmp/blankdb $C15_REQUIREMENTS
