#!/bin/sh

PWM=/sys/class/pwm/pwmchip0/pwm0
cd ${PWM}
echo $(($1 * 10000)) > duty_cycle
