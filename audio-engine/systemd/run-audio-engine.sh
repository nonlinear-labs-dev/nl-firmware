#!/bin/sh

NUM_PERIODS=3
NUM_FRAMES_PER_PERIOD=49

AUDIO_DEVICE=`cat /proc/asound/PCH/pcm0p/info | grep 'card: ' | sed 's/card: //'`
AUDIO_DEVICE=hw:${AUDIO_DEVICE},0

echo "Found Audio Device: ${AUDIO_DEVICE}"

MIDI_DEVICE=`amidi -l | grep -o 'hw:[^ ]*'`

echo "Found MIDI Device: ${MIDI_DEVICE}"

cpupower frequency-set -g performance

./audio-engine -m ${MIDI_DEVICE} -a ${AUDIO_DEVICE} -n ${NUM_PERIODS} -s ${NUM_FRAMES_PER_PERIOD}

