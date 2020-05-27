#!/bin/sh

#
# Author: Anton Schmied
# version : 2.0
#
# ----------- install a ePC Updates from USB-Stick ---------
#
# This scripts will serve the update.tar via thttpd on the BBB and
# restart the ePC, which will evoke the Update

# timeout is so long because the initramFS will potentialy be rebuild which takes quite a while

EPC_IP=$1
TIMEOUT=300

report_and_quit(){
    printf "$1" >> /update/errors.log
    exit $2
}

executeAsRoot() {
    echo "sscl" | /update/utilities/sshpass -p 'sscl' ssh -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no sscl@$EPC_IP "sudo -S /bin/bash -c '$1' "
    return $?
}

wait4playground() {
    for COUNTER in $(seq 1 $TIMEOUT); do
        echo "awaiting reboot ... $COUNTER/$TIMEOUT"
        sleep 1
        executeAsRoot "systemctl status playground" && return 0
    done
    return 1
}


check_preconditions(){
    [ -z "$EPC_IP" ] && report_and_quit "E81: Usage: $EPC_IP <IP-of-ePC> wrong ..." "81"
    ping -c1 $EPC_IP 1>&2 > /dev/null || report_and_quit "E82: Can't ping ePC on $EPC_IP ..." "82"
    executeAsRoot "exit" || report_and_quit "E83: Can't logon to ePC OS ..." "83"
}

update(){
    killall thttpd

    if ! /update/utilities/thttpd -p 8000 -d /update/EPC/ -l /update/EPC/server.log; then
        report_and_quit "E46 ePC update: Could not start http server on BBB..." "46";
    fi

    for RETRYCOUNTER in {1..5}; do
        echo "restarting epc (try $RETRYCOUNTER)"
        executeAsRoot "reboot"

        if ! wait4playground; then
            report_and_quit "E45 ePC update: Reboot taking too long... timed out" "45"
        fi

        if cat /update/EPC/server.log | grep '"GET /update.tar HTTP/1.1" 200'; then
            rm /update/EPC/server.log
            killall thttpd
            return 0
        fi
    done

    killall thttpd
    rm /update/EPC/server.log
    report_and_quit "E47 ePC update: deploying update failed ..." "47"
}

fix_overlay() {
    if executeAsRoot "mount | grep "/lroot:/nloverlay/os-overlay""; then
        if ! executeAsRoot "mkdir /tmp/sda2 \
            && mount /dev/sda2 /tmp/sda2 \
            && sed -i 's@/lroot:/nloverlay/os-overlay@/nloverlay/os-overlay:/lroot@' /tmp/sda2/lib/initcpio/hooks/oroot \
            && mount /tmp/sda2 \
            && mkinitcpio -p linux \
            && mkinitcpio -p linux-rt"; then
                printf "E48 ePC update: fixing Overlay order failed" >> /update/errors.log && return 1
        fi
    fi
    return 0
}


fix_kernel_settings () {
    if ! executeAsRoot "cat /etc/default/grub | grep "isolcpus" > /dev/null"; then
        if ! executeAsRoot "cat /etc/default/grub | grep "mitigations" > /dev/null"; then
            if ! executeAsRoot "mkdir -p /mnt/sda2 \
                && mount /dev/sda2 /mnt/sda2 \
                && mount /dev/sda1 /mnt/sda2/boot \
                && arch-chroot /mnt/sda2 /bin/bash -c "sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\\\"quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2\\\"/g' /etc/default/grub" \
                && arch-chroot /mnt/sda2 /bin/bash -c "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck" \
                && arch-chroot /mnt/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg" \
                && arch-chroot /mnt/sda2 /bin/bash -c "mkdir -p /boot/EFI/BOOT" \
                && arch-chroot /mnt/sda2 /bin/bash -c "cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/BOOT/BOOTX64.EFI" \
                && umount /mnt/sda2/boot \
                && umount /mnt/sda2"; then
                    printf "E49 ePC update: setting Kernel Parameter failed " >> /update/errors.log && return 1
            fi
        fi
    fi
    return 0
}

epc_fix() {
    if ! fix_overlay || ! fix_kernel_settings; then
        report_and_quit "E45 ePC update: Fixing failed" "45"
    fi
    return 0
}

main () {
    check_preconditions
    update
    epc_fix
    return 0
}

main
