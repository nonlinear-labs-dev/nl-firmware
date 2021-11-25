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

COPY /hook/nlhook /lib/initcpio/hooks/
COPY /hook/oroot /lib/initcpio/hooks/
COPY /install/nlhook /lib/initcpio/install/
COPY /install/oroot /lib/initcpio/install/

RUN sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=1/' /etc/default/grub
RUN sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT="quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2"/' /etc/default/grub
RUN sed -i 's/^HOOKS=.*$/HOOKS=\"base udev oroot block filesystems autodetect modconf keyboard net nlhook\"/' /etc/mkinitcpio.conf
RUN sed -i 's/^BINARIES=.*$/BINARIES=\"tar rsync gzip lsblk udevadm\"/' /etc/mkinitcpio.conf
RUN sed -i 's/^MODULES=.*$/MODULES=\"e1000e\"/' /etc/mkinitcpio.conf
RUN sed -i "s/#governor=.*$/governor='performance'/" /etc/default/cpupower
RUN echo "root@192.168.10.11:/mnt/usb-stick  /mnt/usb-stick  fuse.sshfs  sshfs_sync,direct_io,cache=no,reconnect,defaults,_netdev,ServerAliveInterval=2,ServerAliveCountMax=3,StrictHostKeyChecking=off  0  0" >> /etc/fstab
RUN systemctl enable cpupower
RUN systemctl enable sshd
RUN useradd -m sscl
RUN echo 'sscl:sscl' | chpasswd
RUN echo "sscl   ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers



