SET(NL_UPLOAD_PATH /var/www/html/build-tools2) # used with rsync
SET(NL_DOWNLOAD_PATH /build-tools2) # used with http
SET(DOWNLOAD_BASE_URL http://${NL_SERVER}${NL_DOWNLOAD_PATH})
SET(ARCH_SERVER "https://archive.archlinux.org/packages/.all")
SET(RT_SERVER "https://pkgbuild.com/~dvzrv/repos/realtime/x86_64/")

# the iso our distro is based on
SET(EPC2_ARCH_LINUX_VERSION "2020.09.01")
SET(EPC2_BASE_ARCH_DOCKERNAME "epc2-base-arch")
SET(EPC2_BASE_KERNEL "5.9.1.20.arch1-1")
SET(EPC2_BASE_ADDITIONAL_PACKAGES "alsa-lib alsa-tools alsa-utils base boost cpupower efibootmgr freetype2 glibc glibmm grub \
    libpng libsoup linux-firmware linux-rt mc mkinitcpio mkinitcpio-nfs-utils nano networkmanager openssh \
    png++ rsync shared-mime-info sudo sshfs gawk dbus bash")

SET(EPC2_UPDATE_KERNEL "5.15.3.21.realtime1-1")
SET(EPC2_BUILD_TOOLS "gcc git cmake make mc pkgconf wget unzip ccache")
SET(EPC2_UPDATE_PACKAGES "flac lm_sensors dnsmasq arch-install-scripts kexec-tools")
SET(EPC2_VERSIONED_PACKAGES "linux-firmware 20210919.d526e04-1" "kmod 29-1" "glibc 2.33-5")

ADD_SUBDIRECTORY(collect-packages)
ADD_SUBDIRECTORY(fetch-packages)
ADD_SUBDIRECTORY(upload-package-cache)
ADD_SUBDIRECTORY(download-package-cache)
ADD_SUBDIRECTORY(create-update)
ADD_SUBDIRECTORY(create-kernel-update)
ADD_SUBDIRECTORY(build-rootfs)
ADD_SUBDIRECTORY(create-install-medium)
