ARG arch_version
FROM archlinux:${arch_version}
ARG user_id
RUN echo "[dvzrv]" >> /etc/pacman.d/mirrorlist
RUN echo "Server = https://pkgbuild.com/~dvzrv/repo/\$arch" >> /etc/pacman.d/mirrorlist
RUN pacman --noconfirm -Syy
RUN pacman --noconfirm -S sudo
