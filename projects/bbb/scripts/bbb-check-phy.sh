#! /bin/sh

# first we sleep.
# this gives plently of time for linux to mess with the RTC
# (where does it do that set?) before we touch it. Otherwise it
# occasionally updates it right between when we are doing our sets.

# This also gives you a chance to abort in case something goes wrong
# and avoid a infinite loop

REBOOTCOUNT=0
TIMEOUT=5

check_state_loop_hard() {

    for COUNTER in $(seq 1 $TIMEOUT); do
        if [ "$(cat /sys/class/net/eth0/operstate)" = "up" ] || [ "$(cat /sys/class/net/eth0/carrier)" = "1" ]; then
            return 0
        fi
        sleep 1
    done

    echo "$REBOOTCOUNT Ethernet Down - Restarting after $COUNTER/$TIMEOUT tries" >> /usr/C15/scripts/chkphy.log
    grep "" /sys/class/net/eth0/* >> /usr/C15/scripts/chkphy.log
    echo " " >> /usr/C15/scripts/chkphy.log
    sync

    /usr/sbin/bbb-long-reset

    while true;
        do echo waiting to reboot
        sleep 10
    done
}

main() { 
    if [ ! -e /usr/C15/scripts/chkphy.log ]; then
        touch /usr/C15/scripts/chkphy.log
        echo 0 >> /usr/C15/scripts/chkphy.log
    fi

    REBOOTCOUNT=$(sed -n '1p' /usr/C15/scripts/chkphy.log)
    REBOOTCOUNT=$((REBOOTCOUNT+1))
    sed -i "1s/.*/$REBOOTCOUNT/" /usr/C15/scripts/chkphy.log

    check_state_loop_hard
}

main

