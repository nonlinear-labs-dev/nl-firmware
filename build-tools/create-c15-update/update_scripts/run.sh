# !/bin/sh
#
#
# Name:         Anton Schmied
# Date:         21.02.2020
# Version:      2.0

# ePC IP
EPC_IP=192.168.10.10
BBB_IP=192.168.10.11

# general Messages
MSG_DO_NOT_SWITCH_OFF="DO NOT SWITCH OFF C15!"
MSG_REBOOT="PLEASE RESTART C15 NOW"
MSG_CREATING_BACKUP="creating backup "
MSG_UNINSTALLING="uninstalling "
MSG_SAVING_PRESETS="saving presets..."
MSG_RESTORING_PRESETS="restoring presets..."
MSG_UPDATING_RT_FIRMWARE="updating RT firmware..."
MSG_UPDATING_EPC="updating ePC..."
MSG_UPDATING_SYSTEM_FILES="updating system files..."
MSG_UPDATING_BBB="updating BBB..."
MSG_DONE="DONE."
MSG_DONE_WITH_WARNING_CODE="OK. Warning: "
MSG_FAILED_WITH_ERROR_CODE="FAILED! Error Code: "

t2s() {
    /update/utilities/text2soled multitext "$1" "$2" "$3" "$4" "$5" "$6"
}

pretty() {
    echo "$*"
    HEADLINE="$1"
    BOLED_LINE_1="$2"
    BOLED_LINE_2="$3"
    SOLED_LINE_1="$4"
    SOLED_LINE_2="$5"

    t2s "${HEADLINE}@b1c" "${BOLED_LINE_1}@b3c" "${BOLED_LINE_2}@b4c" "${SOLED_LINE_1}@s0c" "${SOLED_LINE_2}@s1c"
}

report_and_quit() {
    pretty "$1" "$2" "$3" "$4" "$5" "$6"
    echo "$2 $3" | systemd-cat -t "C15_Update"
    exit 1
}

epc_update() {
    pretty "" "$MSG_UPDATING_EPC" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_EPC" "$MSG_DO_NOT_SWITCH_OFF"
    chmod +x /update/EPC/epc_update.sh
    /bin/sh /update/EPC/epc_update.sh $EPC_IP

    # error codes 40...49
    return_code=$?
    if [ $return_code -ne 0 ]; then
        pretty "" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_EPC" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1;
    fi

    pretty "" "$MSG_UPDATING_EPC" "$MSG_DONE" "$MSG_UPDATING_EPC" "$MSG_DONE"
    sleep 2
    return 0
}

bbb_update() {
    pretty "" "$MSG_UPDATING_BBB" "$MSG_DO_NOT_SWITCH_OFF" "$MSG_UPDATING_BBB" "$MSG_DO_NOT_SWITCH_OFF"
    chmod +x /update/EPC/epc_update.sh
    /bin/sh /update/EPC/epc_update.sh $EPC_IP $BBB_IP

    # error codes 50...59
    return_code=$?
    if [ $return_code -ne 0 ]; then
        pretty "" "$MSG_UPDATING_BBB" "$MSG_FAILED_WITH_ERROR_CODE $return_code" "$MSG_UPDATING_BBB" "$MSG_FAILED_WITH_ERROR_CODE $return_code"
        sleep 2
        return 1;
    fi

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


# TODO: Fehlerbehandlung
main() {
    rm -f /update/errors.log
    LOG_FILE=/mnt/usb-stick/nonlinear-c15-update.log.txt
    rm -f $LOG_FILE
#    check_preconditions
#    systemctl stop playground          # nur bei 1.5
#    systemctl stop bbbb

    epc_update
    bbb_update
    lpc_update
}

main
