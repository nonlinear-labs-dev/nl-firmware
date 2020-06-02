#! /bin/sh

# first we sleep.
# this gives plently of time for linux to mess with the RTC
# (where does it do that set?) before we touch it. Otherwise it
# occasionally updates it right between when we are doing our sets.

# This also gives you a chance to abort in case something goes wrong
# and avoid a infinite loop

check_state_soft() {
    if [ "$(cat /sys/class/net/eth0/operstate)" = "down" ] || [ "$(cat /sys/class/net/eth0/carrier)" = "0" ]; then

        NOW=$(date +%s)

        echo "chkphy:Rebooting NOW=$NOW"
        echo " State - Soft Rebooting" >>/usr/C15/scripts/chkphy.log
        # OLED output, quick and dirty for better debugging
        /usr/C15/text2soled/text2soled multitext "Ethernet down! @b3c" "RESTARTING C15 soft! @b4c" "Ethernet down! @s1c" "RESTARTING C15 soft! @s2c"
        # make sure that log message actually gets written before we pull the plug
        sync

        reboot

        while true;
            do echo waiting to reboot
            sleep 10
        done
    fi
}

check_state_hard(){
    if [ "$(cat /sys/class/net/eth0/operstate)" = "down" ] || [ "$(cat /sys/class/net/eth0/carrier)" = "0" ]; then

        NOW=$(date +%s)

        echo "chkphy:Rebooting NOW=$NOW"
        echo " State - Hard Rebooting" >>/usr/C15/scripts/chkphy.log
        # OLED output, quick and dirty for better debugging
        /usr/C15/text2soled/text2soled multitext "Ethernet down! @b3c" "RESTARTING C15 hard! @b4c" "Ethernet down! @s1c" "RESTARTING C15 hard! @s2c"
        # make sure that log message actually gets written before we pull the plug
        sync

        /usr/sbin/bbb-long-reset

        while true;
            do echo waiting to reboot
            sleep 10
        done
    fi
}

check_bit_hard(){
    if [ "$(phyreg test 18 13)" = "1" ]; then

        sleep 1

        NOW=$(date +%s)

        echo "chkphy:Rebooting NOW=$NOW"
#  	echo "Rebooting $NOW" >>/var/tmp/chkphy.log
        echo " Rebooting" >>/usr/C15/scripts/chkphy.log
        # OLED output, quick and dirty for better debugging
        /usr/C15/text2soled/text2soled multitext "Ethernet down! @b3c" "RESTARTING C15! @b4c" "Ethernet down! @s1c" "RESTARTING C15! @s2c"
        # make sure that log message actually gets written before we pull the plug
        sync

        /usr/sbin/bbb-long-reset

        while true;
            do echo waiting to reboot
            sleep 10
        done
    fi
}

main() {
    # sleep 30

    rebootCount=$(sed -n '1p' /usr/C15/scripts/chkphy.log)
    rebootCount=$((rebootCount+1))
    sed -i "1s/.*/$rebootCount/" /usr/C15/scripts/chkphy.log
    echo -n "$rebootCount" >>/usr/C15/scripts/chkphy.log

    #check_state_soft
    check_state_hard
    #check_bit_hard

    echo " " >>/usr/C15/scripts/chkphy.log
    echo "chkphy:eth0 good"
}

main

