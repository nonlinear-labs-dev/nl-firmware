#!/bin/sh

mkdir -p /internal/AP-Linux-mnt
mount -o loop /workdir/AP-Linux-V.4.0.iso /internal/AP-Linux-mnt
mkdir -p /internal/AP-Linux-copy
cp -R /internal/AP-Linux-mnt/* /internal/AP-Linux-copy/
chmod 777 /internal/AP-Linux-copy
unsquashfs -no-xattrs /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs
mkdir ./squashfs-root/Audiophile2NonLinux
chmod 777 ./squashfs-root/Audiophile2NonLinux
cp -a /workdir/NonLinux.pkg.tar.gz /sources/hook /sources/install /sources/buildNonlinearLabsBinaries.sh /sources/sda.sfdisk /sources/createUpdateFromRunningOS.sh ./squashfs-root/Audiophile2NonLinux
cp -a /sources/runme.sh ./squashfs-root/etc/profile.d/

mkdir /internal/overlay-scratch /internal/overlay-workdir /internal/overlay-fs
fuse-overlayfs -o lowerdir=/squashfs-root -o upperdir=/internal/overlay-scratch -o workdir=/internal/overlay-workdir /internal/overlay-fs
/internal/overlay-fs/bin/arch-chroot /internal/overlay-fs /bin/bash -c "\

sed -i 's/Required DatabaseOptional/Never/' /etc/pacman.conf
sed -i 's/Server.*mettke/#/' /etc/pacman.d/mirrorlist
mkdir -p /update-packages
tar -C /update-packages -xzf /Audiophile2NonLinux/NonLinux.pkg.tar.gz
rm /Audiophile2NonLinux/NonLinux.pkg.tar.gz
echo 'Server = file:///update-packages/pkg/' > /etc/pacman.d/mirrorlist
mv /update-packages/pkg/gwt-2.8.2 /
"

rm /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs
mksquashfs /internal/overlay-fs /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs
md5sum /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs > /internal/AP-Linux-copy/arch/x86_64/airootfs.md5
xorriso -volid "ARCH_201704" -as mkisofs -no-emul-boot -boot-load-size 4 -boot-info-table -iso-level 4 -b isolinux/isolinux.bin -c isolinux/boot.cat -eltorito-alt-boot -e EFI/archiso/efiboot.img -no-emul-boot -o /internal/NonLinuxAP.iso /internal/AP-Linux-copy/
isohybrid -u /internal/NonLinuxAP.iso
mv /internal/NonLinuxAP.iso /workdir/NonLinuxAP.iso
chown $TARGETUSER /workdir/NonLinuxAP.iso