#!/bin/sh
#
#
# Name:         Anton Schmied
# Date:         2020.05.22
# Version:      2.1
# TODO:         ePC update: fixCPUBindings.sh and fixOverlayOrder.sh

EPC_IP=192.168.10.10
BBB_IP=192.168.10.11

# general Messages
MSG_DO_NOT_SWITCH_OFF="DO NOT SWITCH OFF C15!"
MSG_UPDATING_C15="Updating C15"
MSG_UPDATING_EPC="1/4 Updating..."
MSG_UPDATING_BBB="2/4 Updating..."
MSG_UPDATING_RT_FIRMWARE_1="3/4 Updating..."
MSG_UPDATING_RT_FIRMWARE_2="4/4 Updating..."
MSG_DONE="DONE!"
MSG_FAILED="FAILED!"
MSG_FAILED_WITH_ERROR_CODE="FAILED! Error Code:"
MSG_CHECK_LOG="Please check update log!"
MSG_RESTART="Please Restart!"

ASPECTS="TO_BE_REPLACED_BY_CREATE_C15_UPDATE"

UPDATE_BBB=0
UPDATE_LPC=0
UPDATE_EPC=0

if [[ $ASPECTS = *epc* ]]
then
    UPDATE_EPC=1
    echo "will update epc"
fi

if [[ $ASPECTS = *lpc* ]]
then
    UPDATE_LPC=1
    echo "will update lpc"
fi

if [[ $ASPECTS = *bbb* ]]
then
    UPDATE_BBB=1
    echo "will update bbb"
fi

t2s() {
    /update/utilities/text2soled multitext "$1" "$2" "$3" "$4" "$5" "$6"
}

pretty() {
    echo "$*"
    HEADLINE="$1"
    BOLED_LINE_1="$2"
    BOLED_LINE_2="$3"
    SOLED_LINE_2="$4"
    SOLED_LINE_3="$5"

    t2s "${HEADLINE}@b1c" "${BOLED_LINE_1}@b3c" "${BOLED_LINE_2}@b4c" "${SOLED_LINE_2}@s1c" "${SOLED_LINE_3}@s2c"
}

report() {
    pretty "$1" "$2" "$3" "$2" "$3"
    printf "$2" >> /update/errors.log
    echo "$2" | systemd-cat -t "C15_Update"
}

executeAsRoot() {
    echo "sscl" | /update/utilities/sshpass -p 'sscl' ssh -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no sscl@$EPC_IP "sudo -S /bin/bash -c '$1' 1>&2 > /dev/null"
    return $?
}

executeOnWin() {
    /update/utilities/sshpass -p 'TEST' ssh -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no TEST@$EPC_IP "$1" 1>&2 > /dev/null
    return $?
}

check_preconditions() {
    executeAsRoot "exit"
    if [ $? -ne 0 ]; then
        if [ -z "$EPC_IP" ]; then report "" "E81: Usage: $EPC_IP <IP-of-ePC> wrong ..." "Please retry update!"; return 1; fi
        if ! ping -c1 $EPC_IP 1>&2 > /dev/null; then  report "" "E82: Cannot ping ePC on $EPC_IP ..." "Please retry update!"; return 1; fi
        if executeOnWin "mountvol p: /s & p: & DIR P:\nonlinear"; then
            report "" "E84: Upgrade OS first!" "Please contact NLL!" && return 1
        else
            mount.cifs //192.168.10.10/update /mnt/windows -o user=TEST,password=TEST
            if cat /mnt/windows/ | grep Phase22Renderer.ens; then report "" "E85: OS too old for update!" "Please contact NLL!"; return 1; fi
        fi
        report "" "Something went wrong!" "Please retry update!"
        return 1
    fi
    return 0
}

epc_push_update() {
    chmod +x /update/EPC/epc_push_update.sh
    /bin/sh /update/EPC/epc_push_update.sh $EPC_IP
    return $?
}

epc_pull_update() {
    chmod +x /update/EPC/epc_pull_update.sh
    /bin/sh /update/EPC/epc_pull_update.sh $EPC_IP
    return $?
}

epc_fix() {
    if ! executeAsRoot "mount | grep "/lroot:/nloverlay/os-overlay"" && \
        ! executeAsRoot "mkdir /tmp/sda2 \
        && mount /dev/sda2 /tmp/sda2 \
        && sed -i 's@/lroot:/nloverlay/os-overlay@/nloverlay/os-overlay:/lroot@' /tmp/sda2/lib/initcpio/hooks/oroot \
        && mount /tmp/sda2 \
        && mkinitcpio -p linux \
        && mkinitcpio -p linux-rt"; then
            printf "E48 ePC update: setting Kernel Params failed" >> /update/errors.log; return 48
    fi

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
            printf "E49 ePC update: Fixing Overlay order failed" >> /update/errors.log; return 49
    fi

    return 0
}

epc_update() {
    pretty "" "$MSG_UPDATING_EPC" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_EPC" "$MSG_DO_NOT_SWITCH_OFF"

    if ! epc_push_update; then
        if ! epc_pull_update; then
            pretty "" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
            sleep 2
            return 1
        fi
    fi

    if ! epc_fix; then
        pretty "" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1
    fi

    pretty "" "$MSG_UPDATING_EPC" "$MSG_DONE" "$MSG_UPDATING_EPC" "$MSG_DONE"
    sleep 2
    return 0
}

configure_ssh() {
    echo "Host 192.168.10.10
            StrictHostKeyChecking no
            UserKnownHostsFile=/dev/null
            " > ~/.ssh/config
    chmod 400 ~/.ssh/config
}

bbb_update() {
    pretty "" "$MSG_UPDATING_BBB" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_BBB" "$MSG_DO_NOT_SWITCH_OFF"
    chmod +x /update/BBB/bbb_update.sh
    /bin/sh /update/BBB/bbb_update.sh $EPC_IP $BBB_IP

    # error codes 50...59
    return_code=$?
    if [ $return_code -ne 0 ]; then
        pretty "" "$MSG_UPDATING_BBB" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_BBB" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1;
    fi

    configure_ssh
    pretty "" "$MSG_UPDATING_BBB" "$MSG_DONE" "$MSG_UPDATING_BBB" "$MSG_DONE"

    sleep 2
    return 0
}

lpc_update() {
    pretty "" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_DO_NOT_SWITCH_OFF"
    chmod +x /update/LPC/lpc_update.sh
    rm -f /update/mxli.log

    /bin/sh /update/LPC/lpc_update.sh /update/LPC/M0_project.bin B

    # error codes 30...39
    return_code=$?
    if [ $return_code -ne 0 ]; then
        pretty "" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1;
    fi

    pretty "" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_DONE" "$MSG_UPDATING_RT_FIRMWARE_1" "$MSG_DONE"
    sleep 2

    pretty "" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_DO_NOT_SWITCH_OFF"
    /bin/sh /update/LPC/lpc_update.sh /update/LPC/M4_project.bin A

    # error codes 30...39
    return_code=$?
    if [ $return_code -ne 0 ]; then
        pretty "" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1;
    fi

    pretty "" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_DONE" "$MSG_UPDATING_RT_FIRMWARE_2" "$MSG_DONE"
    sleep 2
    return 0
}

stop_services() {
    systemctl stop playground > /dev/null
    systemctl stop bbbb > /dev/null
    return 0
}

freeze() {
    while true; do
        sleep 1
    done
}

main() {
    rm -f /update/errors.log
    touch /update/errors.log

    configure_ssh
    if ! check_preconditions; then freeze; fi

    [ $UPDATE_BBB == 1 ] && stop_services
    [ $UPDATE_EPC == 1 ] && epc_update
    [ $UPDATE_BBB == 1 ] && bbb_update
    [ $UPDATE_LPC == 1 ] && lpc_update

    if [ $(wc -c /update/errors.log | awk '{print $1}') -ne 0 ]; then
        cp /update/errors.log /mnt/usb-stick/nonlinear-c15-update.log.txt
        pretty "" "$MSG_UPDATING_C15 $MSG_FAILED" "$MSG_CHECK_LOG" "$MSG_UPDATING_C15 $MSG_FAILED" "$MSG_CHECK_LOG"
        freeze
    fi

    pretty "" "$MSG_UPDATING_C15 $MSG_DONE" "$MSG_RESTART" "$MSG_UPDATING_C15 $MSG_DONE" "$MSG_RESTART"
    freeze
}

main
