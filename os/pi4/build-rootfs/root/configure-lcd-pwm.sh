#!/bin/sh
PWM=/sys/class/pwm/pwmchip0

#/usr/bin/dtoverlay pwm pin=12 func=4
#/usr/bin/raspi-gpio set 12 op pn dh

echo 0 > ${PWM}/export
echo 1 > ${PWM}/export

chown -R root:gpio $PWM/*
chmod -R g+rwX $PWM/*

cd ${PWM}/pwm0
# 1kHz --> 1000us period, in ns=1000,000
echo 1000000 > period

# 30% --> "duty cycle" = 3000000
echo 300000 > duty_cycle

echo inversed > polarity

echo 1 > enable

#/usr/bin/raspi-gpio set 12 a0
