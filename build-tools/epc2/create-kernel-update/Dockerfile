FROM archlinux:20200908

RUN pacman -Sy
RUN pacman --noconfirm -S initramfs rsync grub mkinitcpio-nfs-utils
RUN mkdir /packages

COPY install/oroot /lib/initcpio/install/oroot
COPY hook/oroot /lib/initcpio/hooks/oroot
COPY createUpdate.sh /createUpdate.sh
COPY linux-rt-@EPC2_UPDATE_KERNEL@-x86_64.pkg.tar.zst /packages
COPY glibc-2.33-5-x86_64.pkg.tar.zst /packages
COPY kmod-29-1-x86_64.pkg.tar.zst /packages
# COPY linux-firmware-20210919.d526e04-1-any.pkg.tar.zst /packages

RUN sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT="quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2"/' /etc/default/grub
RUN sed -i 's/^HOOKS=.*$/HOOKS=\"base udev oroot block filesystems autodetect modconf keyboard\"/' /etc/mkinitcpio.conf
RUN sed -i 's/^BINARIES=.*$/BINARIES=\"lsblk udevadm\"/' /etc/mkinitcpio.conf
RUN sed -i 's/^MODULES=.*$/MODULES=\"e1000e\"/' /etc/mkinitcpio.conf
#RUN pacman --noconfirm -U /packages/glibc-2.33-5-x86_64.pkg.tar.zst /packages/kmod-29-1-x86_64.pkg.tar.zst /packages/linux-firmware-20210919.d526e04-1-any.pkg.tar.zst /packages/linux-rt-@EPC2_UPDATE_KERNEL@-x86_64.pkg.tar.zst
RUN pacman --noconfirm -U /packages/glibc-2.33-5-x86_64.pkg.tar.zst /packages/kmod-29-1-x86_64.pkg.tar.zst /packages/linux-rt-@EPC2_UPDATE_KERNEL@-x86_64.pkg.tar.zst

