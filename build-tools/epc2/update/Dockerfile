FROM epc2-build-arch
COPY createUpdate.sh /createUpdate.sh
COPY collectUpdatePackages.sh /collectUpdatePackages.sh
RUN mkdir -p /packages
COPY linux-rt-5.15.2.19.realtime1-2-x86_64.pkg.tar.zst /packages/
COPY glibc-2.33-5-x86_64.pkg.tar.zst /packages/
COPY kmod-29-1-x86_64.pkg.tar.zst /packages/
RUN echo "Server = https://archive.archlinux.org/repos/2021/11/01/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist
RUN repo-add /packages/rt.db.tar.gz /packages/linux-rt-5.15.2.19.realtime1-2-x86_64.pkg.tar.zst
RUN pacman -Sy
RUN pacman --noconfirm -S linux-rt archlinux-keyring coreutils initramfs kmod glibc
