

add_custom_command(
    COMMENT "Download AudioPhile-Linux image"
    OUTPUT AP-Linux-V.4.0.iso
    COMMAND wget https://netix.dl.sourceforge.net/project/ap-linux/AP-Linux-V.4.0.iso
)

add_custom_command(
    COMMENT "Download update packages"
    OUTPUT NonLinux.pkg.tar.gz
    COMMAND wget https://github.com/nonlinear-labs-dev/Audiophile2NonLinux/releases/download/1.0/NonLinux.pkg.tar.gz
)

add_custom_command(
    COMMENT "Download update packages"
    OUTPUT NonLinux.pkg.tar.gz
    COMMAND wget https://github.com/nonlinear-labs-dev/Audiophile2NonLinux/releases/download/1.0/NonLinux.pkg.tar.gz
)

echo $FUNCNAME
mkdir -p $STAGING_DIR/original/p1
mkdir -p $STAGING_DIR/original/p2
LOOPDEVICE=$(sudo losetup -f --show -P $ISO_IN)
sudo mount ${LOOPDEVICE}p1 $STAGING_DIR/original/p1
sudo mount ${LOOPDEVICE}p2 $STAGING_DIR/original/p2

add_custom_target(epc-c15 DEPENDS .epc-C15-built)
add_custom_target(epc-rootfs DEPENDS .epc-rootfs-built)
