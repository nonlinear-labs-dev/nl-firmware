# Audiophile2NonLinux
## How to create a NonLinux image file
* In a VM:
  * Download AP Linux ISO image: https://sourceforge.net/projects/ap-linux/
  * git clone this project
  * go into the checked out folder and call 
  ```console
  ./createNonLinuxImage.sh /home/user/Downloads-Folder-Containing/AP-Linux-V.4.0.iso /where/you/wish/the/new/img/AP-Linux-V.4.0.iso
  ```
  * Now, you can either copy the image onto a stick to boot a NUC from, or boot a virtual machine from the image.

## Install NonLinux
* have VirtualBox installed
  ```console
  ./run-me-on-vm-host.sh /where/you/wish/the/new/img/AP-Linux-V.4.0.iso
  ```
* On a NUC Machine
  * attach an USB stick to your PC
  * with lsblk, retieve the device file of the stick, e.g. /dev/sdb
  * call ./createUSBStick.sh /path/to/NonLinux.iso /dev/sdX
  * attach that stick to the NUC and boot
  * tweak BIOS to allow Boot from USB first instead of UEFI Shell. Otherwise the USB has to manually be chosen via UEFI Shell or in BIOS by choosing the bootable device [F10 on Intel in our case]

## How to build our binaries on a NonLinux installation
```console
sudo su
cd /
./buildNonlinearLabsBinaries.sh master
```

Please notice, the binaries and the build files will be gone on next boot.

## How to create an update from the currenlty running OS:
- call 
```console
/createUpdateFromRunningOS.sh
```
- copy the file /update.tar, for example via scp:
```console
scp /update.tar user@computer:
```
