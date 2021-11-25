#!/bin/sh

set -e
set -x

pacman --noconfirm -S $PACKAGES

# ship kernel update
mkdir -p /kernel-update
cp /packages/linux-rt-5.15.2.19.realtime1-2-x86_64.pkg.tar.zst /kernel-update
cp /packages/glibc-2.33-5-x86_64.pkg.tar.zst /kernel-update
cp /packages/kmod-29-1-x86_64.pkg.tar.zst /kernel-update

# trigger kernel update
cat <<- ENDOFHERE > /doKernelUpdate.sh
#!/bin/sh
set -x
if ! uname -a | grep "5.15.2.19"; then
  mkdir -p /mnt/kernel-update
  mount /dev/sda2 /mnt/kernel-update
  mount /dev/sda1 /mnt/kernel-update/boot
  mv /kernel-update /mnt/kernel-update/
  arch-chroot /mnt/kernel-update pacman --noconfirm -U /kernel-update/glibc-2.33-5-x86_64.pkg.tar.zst
  arch-chroot /mnt/kernel-update pacman --noconfirm -U /kernel-update/kmod-29-1-x86_64.pkg.tar.zst
  arch-chroot /mnt/kernel-update pacman --noconfirm -U /kernel-update/linux-rt-5.15.2.19.realtime1-2-x86_64.pkg.tar.zst
  reboot
fi
ENDOFHERE

chmod +x /doKernelUpdate.sh

cat <<- ENDOFHERE > /etc/systemd/system/kernel-update.service
[Unit]
Description=Kernel Update service
Before=audio-engine.service playground.service

[Service]
ExecStart=/bin/sh /doKernelUpdate.sh

[Install]
WantedBy=multi-user.target
ENDOFHERE

systemctl enable kernel-update.service


# build C15 software
mkdir -p /bindir/build
cd /bindir/build

cmake _DCMAKE_INSTALL_DIR=/usr/local/C15 -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off /srcdir
make -j8
make install 

mkdir -p /usr/local/C15/web
tar -xzf /web/web.tar.gz -C /usr/local/C15/web

echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
locale-gen

cat <<- ENDOFHERE > /etc/NetworkManager/system-connections/C15.nmconnection
[connection]
id=C15
uuid=61679179-6804-4197-b476-eacad1d492e4
type=wifi
interface-name=wlp0s20f3
permissions=

[wifi]
band=bg
channel=7
mac-address-blacklist=
mode=ap
ssid=NL-C15-Unit-00000

[wifi-security]
key-mgmt=wpa-psk
pairwise=ccmp;
proto=rsn;
psk=88888888

[ipv4]
address1=192.168.8.2/24,192.168.8.1
dns-search=
method=shared

[ipv6]
addr-gen-mode=stable-privacy
dns-search=
method=auto

[proxy]
ENDOFHERE

chmod 600 /etc/NetworkManager/system-connections/C15.nmconnection
systemctl enable NetworkManager

OUTPUT_OVERLAY_HASH=$(mount | grep -o "upperdir=.*diff," | sed 's/.*overlay2//' | sed 's/diff,/diff/' | head -n1)
OUTPUT_OVERLAY="/host-docker$OUTPUT_OVERLAY_HASH"

echo "OUTPUT_OVERLAY=$OUTPUT_OVERLAY"
ls $OUTPUT_OVERLAY

rm -rf /bindir/update
mkdir -p /bindir/update
tar -C $OUTPUT_OVERLAY --exclude='./srcdir' --exclude='./bindir' --exclude='./web' --exclude='./host-docker' --exclude='./tmp' --exclude='./root' -vczf /bindir/update/NonLinuxOverlay.tar.gz .
touch /bindir/update/$(sha256sum /bindir/update/NonLinuxOverlay.tar.gz | grep -o "^[^ ]*").sign
tar -C /bindir/ -cf /bindir/update.tar update
