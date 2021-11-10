FROM archlinux:20200908

ARG PACKAGES

RUN echo "Server = https://archive.archlinux.org/repos/2020/09/08/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist
RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S wget
RUN mkdir /packages
RUN wget -q http://h2949050.stratoserver.net/build-tools2/linux-rt-5.9.1.20.arch1-1-x86_64.pkg.tar.zst -O /packages/linux-rt-5.9.1.20.arch1-1-x86_64.pkg.tar.zst
RUN repo-add /packages/rt.db.tar.gz /packages/linux-rt-5.9.1.20.arch1-1-x86_64.pkg.tar.zst

RUN echo "[rt]" >> /etc/pacman.conf
RUN echo "SigLevel = Optional TrustAll" >> /etc/pacman.conf
RUN echo "Server=file:///packages/" >> /etc/pacman.conf

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S $PACKAGES



