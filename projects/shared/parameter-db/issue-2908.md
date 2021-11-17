## #2919

### Secondary HW Sources

~~~
When Local is disabled in the MIDI Settings, this control is the
parameter representation of the "Pedal 1" Hardware Source. It
reflects changes that will be sent via MIDI without affecting the
Synth.

This parameter will be visible next to the internal representation
of "Pedal 1" (which reflects changes from the User Interface or
MIDI Input, only affecting the Synth).

Editing the actual "Pedal 1" parameter will not be reflected.
~~~

- [ ] done

## #2920

### Env A/B

#### Attack Velocity

~~~
Velocity influence on the Attack time. The value represents the
logarithmic amount of the reduction (negative values) or gain
(positive values) of the Attack time by high Note-On velocities.
~~~

- [ ] done

> #### [Release Velocity ?]
> 
> ~~~
> Velocity influence on the Release time. The value represents the
> logarithmic amount of the reduction (negative values) or gain
> (positive values) of the Release time by high Note-Off velocities.
> ~~~
> 
> - [x] done

#### Time Key Track

~~~
Key tracking of the attack, decay and release times. The value
determines time reduction (negative values) or gain (positive values)
for higher notes.
~~~

- [ ] done

### Output Mixer

#### Key Pan

~~~
Amount of key panning for each note (referring to the key position
relative to the center at C3 = 60 semitones). At zero, each note
is centered before being passed to the panning section in the mix.
At maximum, low notes will be panned to the left, high notes to the
right. At minimum, the opposite effect applies.
~~~

- [ ] done

### 

## #2921

### Shaper A/B

#### FB - Env C

~~~
Envelope (C) amount for the Feedback Mix. At zero, only a Gate signal
is applied, at higher values, Envelope C is faded in. At negative
values, the Gate signal is faded out, allowing for permanent feedback
at the minimum.
~~~

- [ ] done
