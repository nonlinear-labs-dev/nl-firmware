#!/bin/sh

check_preconditions() {
    echo "Checking preconditions..."
    if [ -z "$1" ]; then
        echo "Checking preconditions failed - please provide branch to build as command line argument."
        return 1
    fi
    echo "Checking preconditions done."
    return 0
}

callChecked() {
    echo "$1..."
    if sh -c "$2"; then
        echo "$1 done."
        return 0
    fi
    echo "$1 failed."
    return 1
}

check_out() {
    callChecked "Checking out project" "git clone https://github.com/nonlinear-labs-dev/C15.git && cd C15 && git checkout $1"
    return $?
}

build(){
    callChecked "Building project" "mkdir build && cd build &&
    cmake -D CMAKE_BUILD_TYPE=Release -D JARSDIR=/gwt-2.8.2  -D BUILD_BBBB=$BBBB_BUILD_SWITCH -D BUILD_AUDIOENGINE=$AE_BUILD_SWITCH -D BUILD_PLAYGROUND=$PG_BUILD_SWITCH ../C15 && 
    make install && systemctl enable /usr/local/etc/systemd/system/audio-engine.service"

    return $?
}

main() {
    if check_preconditions $BRANCH && check_out $BRANCH && build; then
        return 0
    fi
    return 1
}

usage(){
    echo " This script comes with several Arguments" 
    echo ""
    echo "     Usage: 	$0 [ -B BRANCHNAME ] [ -b ON/OFF ] [ -a ON/OFF ] [ -p ON/OFF ] "
    echo "	-B:  	set the Branch"
    echo "	-b:  	disable building BlackBeagleBone-Bridge with OFF"
    echo "	-a:  	disable building Audio-Engine with OFF"
    echo "	-p:  	disable building Playground with OFF"
    echo ""
    echo "	NOTE: 	By default everything will be built!"
    exit 0
}

checkArguments(){
    while getopts hB:b:a:p: opt
    	do
	    case $opt in
	    h) usage;;
            B) BRANCH=${OPTARG};;
    	    b) if [ ${OPTARG} = "OFF" ]; then BBBB_BUILD_SWITCH=${OPTARG}
		    fi
	    ;;
	    a) if [ ${OPTARG} = "OFF" ]; then AE_BUILD_SWITCH=${OPTARG}
		    fi
	    ;;
	    p) if [ ${OPTARG} = "OFF" ]; then PG_BUILD_SWITCH=${OPTARG}
		    fi
            ;;
	    ?) echo "ERROR: no such option!" && usage;;
    	esac
    done
}

BBBB_BUILD_SWITCH="ON"
AE_BUILD_SWITCH="ON"
PG_BUILD_SWITCH="ON"

checkArguments $@ && main
