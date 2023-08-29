# C15 Audio Engine

This project provides the <dfn>C15 Audio Engine</dfn> as an executable application.
The engine can either perform a "CPU burning" test (rendering lots of sines, see CLI Options) or run the C15 Synth (default mode).

## Overview

The main components are:

- io (HighPriorityTasks, Audio and MIDI IO, MIDI Heartbeat)
- recorder (capture and playback of the stereo output of the synth, based on a ringbuffer using the FLAC format)
- synth (handling main event logic for parameters, settings, presets and PlayGround communication; rendering DSP)
- input (EventStage, handling messages from PlayController and MIDI Devices, sending MIDI output)

Like the PlayGround, this project depends on nltools and configuration (defining the "Phase 22" environment).

### Usage

In the corresponding build directory (`build-C15-[build_cfg]/projects/epc/audio/engine`), type:

~~~ console
./audio-engine -a default -m hw:1,0,0
~~~

The application will start using the default AudioDevice and expecting a connected MidiDevice (at hw:1,0,0).
After a fresh compilation, you may want to consider permitting the executable to perform high priority tasks before running  it:

~~~ console
sudo setcap 'cap_sys_nice=eip' ./audio-engine
~~~

See CLI Options below for more info.

## CLI Options

| Option               | CLI Parameter             | CLI Flag  | Description                                                          |
| -------------------- | ------------------------- | --------- | -------------------------------------------------------------------- |
| Sample-Rate          | `--sample-rate`           | `-r`      | Audio samplerate[^1]                                                 |
| Polyphony            | `--polyphony`             | `-p`      | Number of voices[^2]                                                 |
| Midi Input Device    | `--midi-in`               | `-m`      | Name of the ALSA Midi Input Device for raw Midi input                |
| TCD Input Device     | `--tcd-in`                | `-t`      | Name of the ALSA Midi Input Device for TCD input[^3]                 |
| Heartbeat Device     | `--heartbeat`             | `-h`      | Name of the ALSA Midi Output Device used to send heartbeats          |
| Audio Output Device  | `--audio-out`             | `-a`      | Name of the ALSA Audio Output Device                                 |
| Fatal XRuns          | `--fatal-xruns`           | `-f`      | Terminate program in case of ALSA underrun or overrun                |
| Measure Performance  | `--measure-performance`   | `-e`      | Measure AudioEngine realtime performance                             |
| Midi Delay           | `--additional-midi-delay` | `-i`      | Incoming Midi Note Delay (in ns)                                     |
| Frames per Period    | `--frames-per-period`     | `-s`      | ALSA Audio Input Frames per Period                                   |
| Number of Periods    | `--num-periods`           | `-n`      | ALSA Audio Input number of Periods                                   |
| Buffer Size          | `--buffer-size`           | `-b`      | ALSA Audio Input ring buffer size                                    |
| Playground Host      | `--playground-host`       | `-x`      | Where to find the PlayGround                                         |
| Midi Bridge Host     | `--midi-bridge-host`      | `-d`      | Where to find the Midi Bridge                                        |
| Number of Sines      | `--num-sines`             | `-u`      | Do not run the C15Synth, but run a lot of sines in order to burn CPU |

[^1]: currently, only 48000 are supported.
[^2]: currently without effect, always 24 voices.
[^3]: TCD refers to the PlayController realtime protocol. _(The actual TCD protocol is deprecated.)_

## CLI Key Bindings

When running the application, there are some key bindings:

|   Key   | Description                                                                              |
| ------- | ---------------------------------------------------------------------------------------- |
|   `c`   | Prints some status information of the engine for the developer.                          |
|   `e`   | Resets the Synth DSP (stopping Envelopes, flushing Buffers, invoking silence).           |
| ~~`t`~~ | ~~Test Tone[^4]: cycles between three combination modes: Synth-only, Tone-only, Both. ~~ |
|   `i`   | Initialize the Synth with usable Settings and an audible initial Preset.                 |
|   `a`   | After initialization, press a key (note number 60, C3).                                  |
|   `s`   | After initialization, release a key (note number 60, C3).                                |
|   `r`   | Reset performance measure.                                                               |
|   `q`   | Quit the application.                                                                    |

[^4]: The Test Tone is an additional sine tone that can be made audible for testing purposes.
It is connected to the Tune Reference setting (defaulting to 440 Hz).
The initial amplitude is currently hard-coded to -6 dB<sub>FS</sub>.

> [!NOTE]
> The Test Tone[^4] feature has become a proper Setting and is exposed in both UIs. Therefore, it has been deprecated as a CLI key binding.
