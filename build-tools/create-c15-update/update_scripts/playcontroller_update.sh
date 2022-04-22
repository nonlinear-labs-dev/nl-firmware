#!/bin/sh

# verions : 2.0
# changes : append to /update/mxli.log to allow for multiple flashing reports in one file
#
# verions : 2.1
# changes : additions for flashing a number of compatible LPC43xx parts, and removed bank select parameter
#
# ---------- in-service programming the RT firmware into the playcontroller chip -----------
# errors will be logged and reported
#

PLAYCONTROLLER_FIRMWARE=$1
if [ -z "$1" ] ; then
    echo "no argument supplied, expected path to playcontroller firmware blob!"
    echo "E31 RT update: no argument" >> /update/errors.log
    exit 31
fi

if [ ! -f $PLAYCONTROLLER_FIRMWARE ] ; then
    echo "no file in $PLAYCONTROLLER_FIRMWARE"
    echo "E32 RT update: no file" >> /update/errors.log
    exit 32
fi

export_gpio() {
    GPIO=$1
    echo $GPIO > /sys/class/gpio/export
    sleep 0.1
    echo out > /sys/class/gpio/gpio$GPIO/direction
    sleep 0.1
}

unexport_gpio() {
    GPIO=$1
    echo $GPIO > /sys/class/gpio/unexport
    sleep 0.1
}

write_to_gpio() {
    GPIO=$1
    VALUE=$2
    echo $VALUE > /sys/class/gpio/gpio$GPIO/value
    sleep 0.1
}


check_lpc() {
    echo "searching LPC chip..."
    LPC=`/update/utilities/mxli -i -d /dev/ttyUSB0 -N LPC43xx -E -b115200 -T500 -c12000000 -F1kix1 -M1ki@0 -I0 | grep LPC43 | cut -d',' -f1`
    echo "LPC model found: " ${LPC}
}

# --- programming parameters for the different chips ---
LPC4337="-N LPC4337 -F8kix8,64kix7 -B1024,4096 -A0x1A000000,0x1B000000 -M32ki@0x10000000 -I0xA001CA30,0xXXXXXX00 -R0x200@0x1008000,-288@0x10000000 -S8@7"
LPC4323="-N LPC4323 -F8kix8,64kix3 -B1024,4096 -A0x1A000000,0x1B000000 -M32ki@0x10000000 -I0xA00BCB3C,0xXXXXXX44 -R0x200@0x1008000,-288@0x10000000 -S8@7"
# LPC4322 is a special case with mxli 3.0 programming tool:
#  Even though there is only one single FLASH bank at 0x1A000000, the tool fails to program the 4322 when
#  the correct command line is given with only one FLASH bank definition.
#  Therefore a "dummy" 2nd bank has to be specified (and we use the same address for it to make that clear)
LPC4322="-N LPC4322 -F8kix8,64kix7 -B1024,4096 -A0x1A000000,0x1A000000 -M32ki@0x10000000 -I0xA00BCB3C,0xXXXXXX80 -R0x200@0x1008000,-288@0x10000000 -S8@7"

write_blob() {
    # in-service programming of the playcontroller, errors are appended to file "/update/mxli.log"
    case ${LPC} in
    
        "LPC4337" | "LPC4323" | "LPC4322")
            echo "programming..."
            # note: ${!LPC} first expands "LPC" into whatever LPC model string was found by check_lpc(). This matches one of the
            # LPC43xx variable definitions above which is then expanded in a second step.
            /update/utilities/mxli -g -v -d /dev/ttyUSB0 -E -b115200 -T500 -c12000000 ${!LPC} $PLAYCONTROLLER_FIRMWARE  2>> /update/mxli.log
        ;;
        
        *) # default
            echo "LPC model not detected or not supported" >> /update/mxli.log
            return 1;
        ;;
    
    esac

    return $?
}

isp_prefix() {
    write_to_gpio $ISP1 1
    write_to_gpio $ISP2 1
    write_to_gpio $RESET 1
    write_to_gpio $PROGMODE 1
    write_to_gpio $ISP1 0
    write_to_gpio $ISP2 0
    write_to_gpio $RESET 0
    write_to_gpio $RESET 1
    write_to_gpio $ISP1 1
    write_to_gpio $ISP2 1
}

isp_postfix() {
    write_to_gpio $PROGMODE 0
    write_to_gpio $RESET 0
    write_to_gpio $RESET 1
}

#
# -------- code execution starts here 
#

# we handle 2 GPIO pins for the ISP signal so that it works
# with new uniboard revisions where ISP is wired differently.
# Root cause of rewiring is that no all BBB hardware implementations
# correctly handle the GPIOs with our specific device tree and
# a new pin has been choosen that seemingly works with the affected
# BBB hardware implementation
#
# ISP GPIO pin for uniboards < 7.2 (RevD)
ISP1=48
# ISP GPIO pin for uniboards >= 7.2 (RevD)
ISP2=51

RESET=50
PROGMODE=115

echo "setting up access environment..."
export_gpio $ISP1 2> /dev/null
export_gpio $ISP2 2> /dev/null
export_gpio $RESET
export_gpio $PROGMODE


isp_prefix
check_lpc
isp_postfix

isp_prefix
write_blob
return_code=$?
isp_postfix

unexport_gpio $PROGMODE
unexport_gpio $RESET

#we must not unexport ISP lines in order to keep their state
# unexport_gpio $ISP1
# unexport_gpio $ISP2

if [ $return_code -eq 0 ] ; then
    echo "RT update succesful"
    exit 0
else
    echo "RT update failed, see /update/mxli.log"
    echo "E33 RT update: cannot program playcontroller, details:" >> /update/errors.log
    cat /update/mxli.log >> /update/errors.log
    exit 33
fi
