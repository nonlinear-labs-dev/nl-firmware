#!/bin/sh

bindSoundCardIRQ() {
    ALLOW_CPU_2="4"
    for irq in $(cat /proc/interrupts | grep snd_hda_intel | awk '{print $1}' | grep -o "^[0-9]*"); do
        echo "$ALLOW_CPU_2" > /proc/irq/$irq/smp_affinity
    done
}

restrictOtherIRQs() {
    ALLOW_CPU_1_AND_3="a"
    for irq in $(cat /proc/interrupts | grep -v snd_hda_intel | awk '{print $1}' | grep -o "^[0-9]*"); do
        echo "$ALLOW_CPU_1_AND_3" > /proc/irq/$irq/smp_affinity
    done
}

main() {
    bindSoundCardIRQ
    restrictOtherIRQs
}

main
