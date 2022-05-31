#pragma once

/******************************************************************************/
/**	@file       parameter_list.h
    @date       2022-05-31, 15:57
    @version    1.7B-9
    @author     M. Seeber
    @brief      complete list of parameters (and their properties) in TCD order
    @todo
*******************************************************************************/

#include "c15_config.h"
#include "parameter_descriptor.h"

namespace C15 {

const ParameterDescriptor ParameterList[Config::tcd_elements] = {
    {Parameters::Local_Modulateables::Env_A_Att,
     PID::Env_A_Att,
     0.0f,
     {Smoothers::Poly_Slow::Env_A_Att, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Attack Time",
      "Attack", "Time of the (polynomial) Attack segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Dec_1,
     PID::Env_A_Dec_1,
     0.59f,
     {Smoothers::Poly_Slow::Env_A_Dec_1, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Decay 1 Time",
      "Decay 1", "Time of the first (linear) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_BP,
     PID::Env_A_BP,
     0.5f,
     {Smoothers::Poly_Fast::Env_A_BP, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Breakpoint Level",
      "Breakpoint", "Level of the Breakpoint between the two Decay segments."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Dec_2,
     PID::Env_A_Dec_2,
     0.79f,
     {Smoothers::Poly_Slow::Env_A_Dec_2, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Decay 2 Time",
      "Decay 2",
      "Time of the second (exponential) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Sus,
     PID::Env_A_Sus,
     0.0f,
     {Smoothers::Poly_Fast::Env_A_Sus, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Envelope A", "Envelope A", "Sustain Level",
      "Sustain", "Sustain level (target of the second Decay segment)."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Rel,
     PID::Env_A_Rel,
     0.53f,
     {Smoothers::Poly_Slow::Env_A_Rel, None,
      Properties::SmootherScale::Expon_Env_Time, 1.01f, -20.0f, false},
     {101, 1010, 100, 1000, "", "Envelope A", "Envelope A", "Release Time",
      "Release",
      "Time of the (exponential) Release segment in milliseconds (infinite at "
      "maximum)."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Gain,
     PID::Env_A_Gain,
     0.0f,
     {Smoothers::Poly_Fast::Env_A_Gain, None,
      Properties::SmootherScale::Expon_Gain, 24.0f, 0.0f, true},
     {48, 480, 48, 480, "0", "Envelope A", "Envelope A", "Gain", "Gain",
      "Gain [in dB] of the envelope signal. As this signal modulates the "
      "Oscillator and Shaper A, the Gain influences the level and the amount "
      "of phase modulation and distortion."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_A_Lvl_Vel,
     PID::Env_A_Lvl_Vel,
     0.5f,
     {Smoothers::Poly_Sync::Env_A_Lvl_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope A", "Envelope A", "Level Velocity",
      "Level Vel",
      "Influence of the key velocity on the peak, breakpoint and sustain "
      "levels of the envelope [maximum dynamic range in dB]."}},
    {Parameters::Local_Unmodulateables::Env_A_Att_Vel,
     PID::Env_A_Att_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Att_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope A", "Envelope A", "Attack Velocity",
      "Velocity",
      "Velocity influence on the Attack time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Attack time by high Note-On velocities."}},
    {Parameters::Local_Unmodulateables::Env_A_Rel_Vel,
     PID::Env_A_Rel_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Rel_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope A", "Envelope A", "Release Velocity",
      "Velocity",
      "Velocity influence on the Release time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Release time by high Note-Off velocities."}},
    {Parameters::Local_Unmodulateables::Env_A_Lvl_KT,
     PID::Env_A_Lvl_KT,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Lvl_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope A", "Envelope A", "Level Key Trk",
      "Level KT",
      "Key tracking of the envelope's peak, breakpoint and sustain levels [dB "
      "per semitone].\nPositive values: higher levels for higher notes (+1.0 = "
      "+12 dB per octave).\nNegative values: lower levels for higher notes "
      "(-1.0 = -12 dB per octave).\nOrigin at C3 = 60 semitones."}},
    {Parameters::Local_Unmodulateables::Env_A_Time_KT,
     PID::Env_A_Time_KT,
     0.05f,
     {Smoothers::Poly_Sync::Env_A_Time_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope A", "Envelope A", "Time Key Trk",
      "Time KT",
      "Key tracking of the attack, decay and release times. The value "
      "determines time reduction (positive values) or gain (negative values) "
      "for higher notes."}},
    {Parameters::Local_Modulateables::Env_B_Att,
     PID::Env_B_Att,
     0.0f,
     {Smoothers::Poly_Slow::Env_B_Att, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Attack Time",
      "Attack", "Time of the (polynomial) Attack segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Dec_1,
     PID::Env_B_Dec_1,
     0.59f,
     {Smoothers::Poly_Slow::Env_B_Dec_1, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Decay 1 Time",
      "Decay 1", "Time of the first (linear) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_BP,
     PID::Env_B_BP,
     0.5f,
     {Smoothers::Poly_Fast::Env_B_BP, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Breakpoint Level",
      "Breakpoint", "Level of the Breakpoint between the two Decay segments."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Dec_2,
     PID::Env_B_Dec_2,
     0.79f,
     {Smoothers::Poly_Slow::Env_B_Dec_2, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Decay 2 Time",
      "Decay 2",
      "Time of the second (exponential) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Sus,
     PID::Env_B_Sus,
     0.0f,
     {Smoothers::Poly_Fast::Env_B_Sus, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Envelope B", "Envelope B", "Sustain Level",
      "Sustain", "Sustain level (target of the second Decay segment)."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Rel,
     PID::Env_B_Rel,
     0.53f,
     {Smoothers::Poly_Slow::Env_B_Rel, None,
      Properties::SmootherScale::Expon_Env_Time, 1.01f, -20.0f, false},
     {101, 1010, 100, 1000, "", "Envelope B", "Envelope B", "Release Time",
      "Release",
      "Time of the (exponential) Release segment in milliseconds (infinite at "
      "maximum)."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Gain,
     PID::Env_B_Gain,
     0.0f,
     {Smoothers::Poly_Fast::Env_B_Gain, None,
      Properties::SmootherScale::Expon_Gain, 24.0f, 0.0f, true},
     {48, 480, 48, 480, "0", "Envelope B", "Envelope B", "Gain", "Gain",
      "Gain [in dB] of the envelope signal. As this signal modulates the "
      "Oscillator and Shaper B, the Gain influences the level and the amount "
      "of phase modulation and distortion."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_B_Lvl_Vel,
     PID::Env_B_Lvl_Vel,
     0.5f,
     {Smoothers::Poly_Sync::Env_B_Lvl_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope B", "Envelope B", "Level Velocity",
      "Level Vel",
      "Influence of the key velocity on the peak, breakpoint and sustain "
      "levels of the envelope [maximum dynamic range in dB]."}},
    {Parameters::Local_Unmodulateables::Env_B_Att_Vel,
     PID::Env_B_Att_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Att_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope B", "Envelope B", "Attack Velocity",
      "Velocity",
      "Velocity influence on the Attack time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Attack time by high Note-On velocities."}},
    {Parameters::Local_Unmodulateables::Env_B_Rel_Vel,
     PID::Env_B_Rel_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Rel_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope B", "Envelope B", "Release Velocity",
      "Velocity",
      "Velocity influence on the Release time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Release time by high Note-Off velocities."}},
    {Parameters::Local_Unmodulateables::Env_B_Lvl_KT,
     PID::Env_B_Lvl_KT,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Lvl_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope B", "Envelope B", "Level Key Trk",
      "Level KT",
      "Key tracking of the envelope's peak, breakpoint and sustain levels [dB "
      "per semitone].\nPositive values: higher levels for higher notes (+1.0 = "
      "+12 dB per octave).\nNegative values: lower levels for higher notes "
      "(-1.0 = -12 dB per octave).\nOrigin at C3 = 60 semitones."}},
    {Parameters::Local_Unmodulateables::Env_B_Time_KT,
     PID::Env_B_Time_KT,
     0.05f,
     {Smoothers::Poly_Sync::Env_B_Time_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope B", "Envelope B", "Time Key Trk",
      "Time KT",
      "Key tracking of the attack, decay and release times. The value "
      "determines time reduction (positive values) or gain (negative values) "
      "for higher notes."}},
    {Parameters::Local_Modulateables::Env_C_Att,
     PID::Env_C_Att,
     0.0f,
     {Smoothers::Poly_Slow::Env_C_Att, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope C", "Envelope C", "Attack Time",
      "Attack", "Time of the (polynomial) Attack segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_C_Dec_1,
     PID::Env_C_Dec_1,
     0.59f,
     {Smoothers::Poly_Slow::Env_C_Dec_1, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope C", "Envelope C", "Decay 1 Time",
      "Decay 1", "Time of the first (linear) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_C_BP,
     PID::Env_C_BP,
     0.5f,
     {Smoothers::Poly_Fast::Env_C_BP, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope C", "Envelope C", "Breakpoint Level",
      "Breakpoint",
      "Level of the (bipolar) Breakpoint between the two Decay segments."}},
    {None},
    {Parameters::Local_Modulateables::Env_C_Dec_2,
     PID::Env_C_Dec_2,
     0.79f,
     {Smoothers::Poly_Slow::Env_C_Dec_2, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Envelope C", "Envelope C", "Decay 2 Time",
      "Decay 2",
      "Time of the second (exponential) Decay segment in milliseconds."}},
    {None},
    {Parameters::Local_Modulateables::Env_C_Rel,
     PID::Env_C_Rel,
     0.53f,
     {Smoothers::Poly_Slow::Env_C_Rel, None,
      Properties::SmootherScale::Expon_Env_Time, 1.01f, -20.0f, false},
     {101, 1010, 100, 1000, "", "Envelope C", "Envelope C", "Release Time",
      "Release",
      "Time of the (exponential) Release segment in milliseconds (infinite at "
      "maximum)."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_C_Lvl_Vel,
     PID::Env_C_Lvl_Vel,
     0.5f,
     {Smoothers::Poly_Sync::Env_C_Lvl_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope C", "Envelope C", "Level Velocity",
      "Level Vel",
      "Influence of the key velocity on the peak, breakpoint and sustain "
      "levels of the envelope [maximum dynamic range in dB]."}},
    {Parameters::Local_Unmodulateables::Env_C_Att_Vel,
     PID::Env_C_Att_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_C_Att_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope C", "Envelope C", "Attack Velocity",
      "Velocity",
      "Velocity influence on the Attack time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Attack time by high Note-On velocities."}},
    {Parameters::Local_Unmodulateables::Env_C_Rel_Vel,
     PID::Env_C_Rel_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_C_Rel_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope C", "Envelope C", "Release Velocity",
      "Velocity",
      "Velocity influence on the Release time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Release time by high Note-Off velocities."}},
    {Parameters::Local_Unmodulateables::Env_C_Lvl_KT,
     PID::Env_C_Lvl_KT,
     0.0f,
     {Smoothers::Poly_Sync::Env_C_Lvl_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope C", "Envelope C", "Level Key Trk",
      "Level KT",
      "Key tracking of the envelope's peak, breakpoint and sustain levels [dB "
      "per semitone].\nPositive values: higher levels for higher notes (+1.0 = "
      "+12 dB per octave).\nNegative values: lower levels for higher notes "
      "(-1.0 = -12 dB per octave).\nOrigin at C3 = 60 semitones."}},
    {Parameters::Local_Unmodulateables::Env_C_Time_KT,
     PID::Env_C_Time_KT,
     0.05f,
     {Smoothers::Poly_Sync::Env_C_Time_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Envelope C", "Envelope C", "Time Key Trk",
      "Time KT",
      "Key tracking of the attack, decay and release times. The value "
      "determines time reduction (positive values) or gain (negative values) "
      "for higher notes."}},
    {Parameters::Local_Modulateables::Osc_A_Pitch,
     PID::Osc_A_Pitch,
     80.0f / 150.0f,
     {Smoothers::Poly_Slow::Osc_A_Pitch, None,
      Properties::SmootherScale::Linear, 150.0f, -20.0f, false},
     {150, 15000, 150, 15000, "", "Oscillator A", "Oscillator A", "Pitch",
      "Pitch",
      "Pitch (logarithmic frequency) of Oscillator A at C3 (MIDI note 60) [in "
      "semitones, based on MIDI note numbers]. The range below zero is shaped "
      "in the way that -20 corresponds to 0 Hz."}},
    {None},
    {Parameters::Local_Unmodulateables::Osc_A_Pitch_KT,
     PID::Osc_A_Pitch_KT,
     0.5f,
     {Smoothers::Poly_Slow::Osc_A_Pitch_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 20000, None, None, "", "Oscillator A", "Oscillator A",
      "Pitch - Key Trk", "Key Trk",
      "Key tracking of the oscillator pitch. It's the scaling factor between "
      "the key position of a received MIDI note (relative to C3 = 60 "
      "semitones) and the pitch of the oscillator.\nAt 100.0 %, the pitch "
      "follows the equal-tempered scale (when the Scale group is "
      "equal-tempered as well). At values larger than 100.0 %, the tuning will "
      "be stretched.\nAt 50.0 %, a quartertone scale emerges and at 0.0 %, the "
      "pitch is constant for all keys."}},
    {Parameters::Local_Modulateables::Osc_A_Pitch_Env_C,
     PID::Osc_A_Pitch_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_Pitch_Env_C, None,
      Properties::SmootherScale::Linear, 80.0f, 0.0f, true},
     {80, 8000, 80, 8000, "", "Oscillator A", "Oscillator A", "Pitch - Env C",
      "Env C", "Amount of pitch modulation by Envelope C [in semitones]."}},
    {Parameters::Local_Modulateables::Osc_A_Fluct,
     PID::Osc_A_Fluct,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_Fluct, None,
      Properties::SmootherScale::Parabolic, 0.95f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Oscillator A", "Oscillator A", "Fluctuation",
      "Fluctuation",
      "Fluctuation of the oscillator frequency. If the parameter is set larger "
      "than 0.0 %, the frequency is changed at the beginning of each "
      "oscillation period by a random amount. At 100.0 %, the maximum "
      "frequency variation is +/- 95 %."}},
    {None},
    {Parameters::Local_Modulateables::Osc_A_Fluct_Env_C,
     PID::Osc_A_Fluct_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_Fluct_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A", "Fluct - Env C",
      "Env C",
      "Amount of modulation of the frequency fluctuation by Envelope C. At "
      "zero, the modulation stays constant, at higher values, the envelope "
      "applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_A_PM_Self,
     PID::Osc_A_PM_Self,
     0.0f,
     {Smoothers::Poly_Fast::Osc_A_PM_Self, None,
      Properties::SmootherScale::Parabolic, 0.5f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator A", "Oscillator A", "PM Self",
      "PM Self",
      "Amount of phase modulation by Oscillator & Shaper A (local feedback)."}},
    {None},
    {Parameters::Local_Modulateables::Osc_A_PM_Self_Env_A,
     PID::Osc_A_PM_Self_Env_A,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_PM_Self_Env_A, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A",
      "PM Self - Env A", "Env A",
      "Envelope (A) amount for the phase modulation by Oscillator & Shaper A "
      "(local feedback). At zero, the modulation stays constant, at higher "
      "values, the envelope applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_A_PM_Self_Shp,
     PID::Osc_A_PM_Self_Shp,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_PM_Self_Shp,
      Signals::Quasipoly_Signals::Osc_A_PM_Self_Shp,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A",
      "PM Self - Shaper", "Shaper",
      "Mix amount of Shaper A in the signal being used for phase modulation "
      "(self). At zero, the output signal of Oscillator A is used. At negative "
      "values, the signal from the Shaper is inverted."}},
    {Parameters::Local_Modulateables::Osc_A_PM_B,
     PID::Osc_A_PM_B,
     0.0f,
     {Smoothers::Poly_Fast::Osc_A_PM_B, None,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator A", "Oscillator A", "PM B", "PM B",
      "Amount of phase modulation by Oscillator & Shaper B (cross feedback)."}},
    {None},
    {Parameters::Local_Modulateables::Osc_A_PM_B_Env_B,
     PID::Osc_A_PM_B_Env_B,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_PM_B_Env_B, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A", "PM B - Env B",
      "Env B",
      "Envelope (B) amount for the pahse modulation by Oscillator & Shaper B "
      "(cross feedback). At zero, the modulation stays constant, at higher "
      "values, the envelope applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_A_PM_B_Shp,
     PID::Osc_A_PM_B_Shp,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_PM_B_Shp,
      Signals::Quasipoly_Signals::Osc_A_PM_B_Shp,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A", "PM B - Shaper",
      "Shaper",
      "Mix amount of Shaper B in the signal being used for phase modulation (B "
      "-> A). At zero, the output signal of Oscillator B is used. At negative "
      "values, the signal from the Shaper is inverted."}},
    {Parameters::Local_Modulateables::Osc_A_PM_FB,
     PID::Osc_A_PM_FB,
     0.0f,
     {Smoothers::Poly_Fast::Osc_A_PM_FB, None,
      Properties::SmootherScale::Parabolic, 0.5f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator A", "Oscillator A", "PM FB",
      "PM FB", "Amount of phase modulation by the Feedback signal."}},
    {None},
    {Parameters::Local_Modulateables::Osc_A_PM_FB_Env_C,
     PID::Osc_A_PM_FB_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_A_PM_FB_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator A", "Oscillator A", "PM FB - Env C",
      "Env C",
      "Envelope (C) amount for the phase modulation by the Feedback signal. At "
      "zero, the modulation stays constant, at higher values, the envelope "
      "applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Shp_A_Drive,
     PID::Shp_A_Drive,
     0.2f,
     {Smoothers::Poly_Fast::Shp_A_Drive, None,
      Properties::SmootherScale::Expon_Shaper_Drive, 50.0f, 0.18f, false},
     {100, 500, 50, 500, "", "Shaper A", "Shaper A", "Drive", "Drive",
      "Input gain [in dB] of the sine shaper stage. Higher gains will create "
      "more distortion and harmonics."}},
    {None},
    {Parameters::Local_Modulateables::Shp_A_Drive_Env_A,
     PID::Shp_A_Drive_Env_A,
     0.0f,
     {Smoothers::Poly_Slow::Shp_A_Drive_Env_A, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper A", "Shaper A", "Drive - Env A",
      "Env A",
      "Envelope (A) amount for the Drive factor. At zero, the gain stays "
      "constant, at higher values, the envelope applies a time-variant "
      "attenuation."}},
    {Parameters::Local_Modulateables::Shp_A_Fold,
     PID::Shp_A_Fold,
     0.5f,
     {Smoothers::Poly_Fast::Shp_A_Fold, Signals::Quasipoly_Signals::Shp_A_Fold,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper A", "Shaper A", "Fold", "Fold",
      "Amount of folding back of the shaper curve for high input "
      "amplitudes.\n0.0 %: flat saturation, no folding\n100.0 %: fully folded "
      "back (periodic sine curve)\nA higher amount of folding leads to a "
      "softer but more nasal sound."}},
    {Parameters::Local_Modulateables::Shp_A_Asym,
     PID::Shp_A_Asym,
     0.0f,
     {Smoothers::Poly_Fast::Shp_A_Asym, Signals::Quasipoly_Signals::Shp_A_Asym,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper A", "Shaper A", "Asymmetry", "Asym",
      "Asymmetry of the shaper curve, generating even (2nd, 4th, ...) "
      "harmonics. At higher values it becomes a parabolic curve that shifts "
      "the frequency of the fundamental to its double."}},
    {Parameters::Local_Modulateables::Shp_A_Mix,
     PID::Shp_A_Mix,
     0.0f,
     {Smoothers::Poly_Fast::Shp_A_Mix, Signals::Quasipoly_Signals::Shp_A_Mix,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Shaper A", "Shaper A", "Mix", "Mix",
      "Mix amount of Shaper A in the signal sent to the Filters and to the "
      "Output Mixer. At zero, it is the input signal of the Shaper - behind FB "
      "Mix. At negative values, the signal from the Shaper is inverted."}},
    {None},
    {Parameters::Local_Modulateables::Shp_A_FB_Mix,
     PID::Shp_A_FB_Mix,
     0.0f,
     {Smoothers::Poly_Fast::Shp_A_FB_Mix,
      Signals::Quasipoly_Signals::Shp_A_FB_Mix,
      Properties::SmootherScale::Cubic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Shaper A", "Shaper A", "FB Mix", "FB Mix",
      "Crossfades between Oscillator & Shaper A (at zero) and the Feedback "
      "signal for the signal A."}},
    {None},
    {Parameters::Local_Modulateables::Shp_A_FB_Env_C,
     PID::Shp_A_FB_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Shp_A_FB_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Shaper A", "Shaper A", "FB - Gate-Env C",
      "Gate-Env C",
      "Envelope (C) amount for the Feedback Mix. At zero, only a Gate signal "
      "is applied, at higher values, Envelope C is faded in. At negative "
      "values, the Gate signal is faded out, allowing for permanent feedback "
      "at the minimum."}},
    {Parameters::Local_Modulateables::Shp_A_Ring_Mod,
     PID::Shp_A_Ring_Mod,
     0.0f,
     {Smoothers::Poly_Fast::Shp_A_Ring_Mod,
      Signals::Quasipoly_Signals::Shp_A_Ring_Mod,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Shaper A", "Shaper A", "Ring Mod", "Ring Mod",
      "Mix amount of the ring modulation between both Oscillators & Shapers."}},
    {None},
    {Parameters::Local_Modulateables::Osc_B_Pitch,
     PID::Osc_B_Pitch,
     80.0f / 150.0f,
     {Smoothers::Poly_Slow::Osc_B_Pitch, None,
      Properties::SmootherScale::Linear, 150.0f, -20.0f, false},
     {150, 15000, 150, 15000, "", "Oscillator B", "Oscillator B", "Pitch",
      "Pitch",
      "Pitch (logarithmic frequency) of Oscillator B at C3 (MIDI note 60) [in "
      "semitones, based on MIDI note numbers]. The range below zero is shaped "
      "in the way that -20 corresponds to 0 Hz."}},
    {None},
    {Parameters::Local_Unmodulateables::Osc_B_Pitch_KT,
     PID::Osc_B_Pitch_KT,
     0.5f,
     {Smoothers::Poly_Slow::Osc_B_Pitch_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 20000, None, None, "", "Oscillator B", "Oscillator B",
      "Pitch - Key Trk", "Key Trk",
      "Key tracking of the oscillator pitch. It's the scaling factor between "
      "the key position of a received MIDI note (relative to C3 = 60 "
      "semitones) and the pitch of the oscillator.\nAt 100.0 %, the pitch "
      "follows the equal-tempered scale (when the Scale group is "
      "equal-tempered as well). At values larger than 100.0 %, the tuning will "
      "be stretched.\nAt 50.0 %, a quartertone scale emerges and at 0.0 %, the "
      "pitch is constant for all keys."}},
    {Parameters::Local_Modulateables::Osc_B_Pitch_Env_C,
     PID::Osc_B_Pitch_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_Pitch_Env_C, None,
      Properties::SmootherScale::Linear, 80.0f, 0.0f, true},
     {80, 8000, 80, 8000, "", "Oscillator B", "Oscillator B", "Pitch - Env C",
      "Env C", "Amount of pitch modulation by Envelope C [in semitones]."}},
    {Parameters::Local_Modulateables::Osc_B_Fluct,
     PID::Osc_B_Fluct,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_Fluct, None,
      Properties::SmootherScale::Parabolic, 0.95f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Oscillator B", "Oscillator B", "Fluctuation",
      "Fluctuation",
      "Fluctuation of the oscillator frequency. If the parameter is set larger "
      "than 0.0 %, the frequency is changed at the beginning of each "
      "oscillation period by a random amount. At 100.0 %, the maximum "
      "frequency variation is +/- 95 %."}},
    {None},
    {Parameters::Local_Modulateables::Osc_B_Fluct_Env_C,
     PID::Osc_B_Fluct_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_Fluct_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B", "Fluct - Env C",
      "Env C",
      "Amount of modulation of the frequency fluctuation by Envelope C. At "
      "zero, the modulation stays constant, at higher values, the envelope "
      "applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_B_PM_Self,
     PID::Osc_B_PM_Self,
     0.0f,
     {Smoothers::Poly_Fast::Osc_B_PM_Self, None,
      Properties::SmootherScale::Parabolic, 0.5f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator B", "Oscillator B", "PM Self",
      "PM Self",
      "Amount of phase modulation by Oscillator & Shaper B (local feedback)."}},
    {None},
    {Parameters::Local_Modulateables::Osc_B_PM_Self_Env_B,
     PID::Osc_B_PM_Self_Env_B,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_PM_Self_Env_B, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B",
      "PM Self - Env B", "Env B",
      "Envelope (B) amount for the phase modulation by Oscillator & Shaper B "
      "(local feedback). At zero, the modulation stays constant, at higher "
      "values, the envelope applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_B_PM_Self_Shp,
     PID::Osc_B_PM_Self_Shp,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_PM_Self_Shp,
      Signals::Quasipoly_Signals::Osc_B_PM_Self_Shp,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B",
      "PM Self - Shaper", "Shaper",
      "Mix amount of Shaper B in the signal being used for phase modulation "
      "(self). At zero, the output signal of Oscillator B is used. At negative "
      "values, the signal from the Shaper is inverted."}},
    {Parameters::Local_Modulateables::Osc_B_PM_A,
     PID::Osc_B_PM_A,
     0.0f,
     {Smoothers::Poly_Fast::Osc_B_PM_A, None,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator B", "Oscillator B", "PM A", "PM A",
      "Amount of phase modulation by Oscillator & Shaper A (cross feedback)."}},
    {None},
    {Parameters::Local_Modulateables::Osc_B_PM_A_Env_A,
     PID::Osc_B_PM_A_Env_A,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_PM_A_Env_A, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B", "PM A - Env A",
      "Env A",
      "Envelope (A) amount for the pahse modulation by Oscillator & Shaper A "
      "(cross feedback). At zero, the modulation stays constant, at higher "
      "values, the envelope applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Osc_B_PM_A_Shp,
     PID::Osc_B_PM_A_Shp,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_PM_A_Shp,
      Signals::Quasipoly_Signals::Osc_B_PM_A_Shp,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B", "PM A - Shaper",
      "Shaper",
      "Mix amount of Shaper A in the signal being used for phase modulation (A "
      "-> B). At zero, the output signal of Oscillator A is used. At negative "
      "values, the signal from the Shaper is inverted."}},
    {Parameters::Local_Modulateables::Osc_B_PM_FB,
     PID::Osc_B_PM_FB,
     0.0f,
     {Smoothers::Poly_Fast::Osc_B_PM_FB, None,
      Properties::SmootherScale::Parabolic, 0.5f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Oscillator B", "Oscillator B", "PM FB",
      "PM FB", "Amount of phase modulation by the Feedback signal."}},
    {None},
    {Parameters::Local_Modulateables::Osc_B_PM_FB_Env_C,
     PID::Osc_B_PM_FB_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Osc_B_PM_FB_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Oscillator B", "Oscillator B", "PM FB - Env C",
      "Env C",
      "Envelope (C) amount for the phase modulation by the Feedback signal. At "
      "zero, the modulation stays constant, at higher values, the envelope "
      "applies a time-variant attenuation."}},
    {Parameters::Local_Modulateables::Shp_B_Drive,
     PID::Shp_B_Drive,
     0.2f,
     {Smoothers::Poly_Fast::Shp_B_Drive, None,
      Properties::SmootherScale::Expon_Shaper_Drive, 50.0f, 0.18f, false},
     {100, 500, 50, 500, "", "Shaper B", "Shaper B", "Drive", "Drive",
      "Input gain [in dB] of the sine shaper stage. Higher gains will create "
      "more distortion and harmonics."}},
    {None},
    {Parameters::Local_Modulateables::Shp_B_Drive_Env_B,
     PID::Shp_B_Drive_Env_B,
     0.0f,
     {Smoothers::Poly_Slow::Shp_B_Drive_Env_B, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper B", "Shaper B", "Drive - Env B",
      "Env B",
      "Envelope (B) amount for the Drive factor. At zero, the gain stays "
      "constant, at higher values, the envelope applies a time-variant "
      "attenuation."}},
    {Parameters::Local_Modulateables::Shp_B_Fold,
     PID::Shp_B_Fold,
     0.5f,
     {Smoothers::Poly_Fast::Shp_B_Fold, Signals::Quasipoly_Signals::Shp_B_Fold,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper B", "Shaper B", "Fold", "Fold",
      "Amount of folding back of the shaper curve for high input "
      "amplitudes.\n0.0 %: flat saturation, no folding\n100.0 %: fully folded "
      "back (periodic sine curve)\nA higher amount of folding leads to a "
      "softer but more nasal sound."}},
    {Parameters::Local_Modulateables::Shp_B_Asym,
     PID::Shp_B_Asym,
     0.0f,
     {Smoothers::Poly_Fast::Shp_B_Asym, Signals::Quasipoly_Signals::Shp_B_Asym,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Shaper B", "Shaper B", "Asymmetry", "Asym",
      "Asymmetry of the shaper curve, generating even (2nd, 4th, ...) "
      "harmonics. At higher values it becomes a parabolic curve that shifts "
      "the frequency of the fundamental to its double."}},
    {Parameters::Local_Modulateables::Shp_B_Mix,
     PID::Shp_B_Mix,
     0.0f,
     {Smoothers::Poly_Fast::Shp_B_Mix, Signals::Quasipoly_Signals::Shp_B_Mix,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Shaper B", "Shaper B", "Mix", "Mix",
      "Mix amount of Shaper B in the signal sent to the Filters and to the "
      "Output Mixer. At zero, it is the input signal of the Shaper - behind FB "
      "Mix. At negative values, the signal from the Shaper is inverted."}},
    {None},
    {Parameters::Local_Modulateables::Shp_B_FB_Mix,
     PID::Shp_B_FB_Mix,
     0.0f,
     {Smoothers::Poly_Fast::Shp_B_FB_Mix,
      Signals::Quasipoly_Signals::Shp_B_FB_Mix,
      Properties::SmootherScale::Cubic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Shaper B", "Shaper B", "FB Mix", "FB Mix",
      "Crossfades between Oscillator & Shaper B (at zero) and the Feedback "
      "signal for the signal B."}},
    {None},
    {Parameters::Local_Modulateables::Shp_B_FB_Env_C,
     PID::Shp_B_FB_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Shp_B_FB_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Shaper B", "Shaper B", "FB - Gate-Env C",
      "Gate-Env C",
      "Envelope (C) amount for the Feedback Mix. At zero, only a Gate signal "
      "is applied, at higher values, Envelope C is faded in. At negative "
      "values, the Gate signal is faded out, allowing for permanent feedback "
      "at the minimum."}},
    {Parameters::Local_Modulateables::Shp_B_Ring_Mod,
     PID::Shp_B_Ring_Mod,
     0.0f,
     {Smoothers::Poly_Fast::Shp_B_Ring_Mod,
      Signals::Quasipoly_Signals::Shp_B_Ring_Mod,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Shaper B", "Shaper B", "Ring Mod", "Ring Mod",
      "Mix amount of the ring modulation between both Oscillators & Shapers."}},
    {None},
    {Parameters::Local_Modulateables::Comb_Flt_In_A_B,
     PID::Comb_Flt_In_A_B,
     0.0f,
     {Smoothers::Poly_Fast::Comb_Flt_In_A_B,
      Signals::Quasipoly_Signals::Comb_Flt_In_A_B,
      Properties::SmootherScale::S_Curve, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "A - B", "A - B",
      "The signal for the Comb Filter as a crossfade between the outputs of "
      "Oscillator & Shaper A and Oscillator & Shaper B."}},
    {None},
    {Parameters::Local_Modulateables::Comb_Flt_Pitch,
     PID::Comb_Flt_Pitch,
     0.5f,
     {Smoothers::Poly_Slow::Comb_Flt_Pitch, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 120.0f, 0.0f, false},
     {120, 12000, 120, 12000, "", "Comb Filter", "Comb Filter", "Pitch",
      "Pitch",
      "Coarse pitch of the Comb Filter (delay) at C3 (MIDI note 60) [in "
      "semitones, based on MIDI note numbers]."}},
    {None},
    {Parameters::Local_Unmodulateables::Comb_Flt_Pitch_KT,
     PID::Comb_Flt_Pitch_KT,
     0.5f,
     {Smoothers::Poly_Slow::Comb_Flt_Pitch_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 20000, None, None, "", "Comb Filter", "Comb Filter",
      "Pitch - Key Trk", "Key Trk",
      "Key scaling of the tuning of the Comb Filter (delay).\n0.0 %: same "
      "tuning for all keys\n100.0 %: full tracking with keys, origin at C3 = "
      "60 semitones"}},
    {Parameters::Local_Modulateables::Comb_Flt_Pitch_Env_C,
     PID::Comb_Flt_Pitch_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_Pitch_Env_C, None,
      Properties::SmootherScale::Linear, 80.0f, 0.0f, true},
     {80, 8000, 80, 8000, "", "Comb Filter", "Comb Filter", "Pitch - Env C",
      "Env C",
      "Amount of modulation of the Comb Filter pitch by Envelope C [in "
      "semitones]."}},
    {Parameters::Local_Modulateables::Comb_Flt_Decay,
     PID::Comb_Flt_Decay,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_Decay, None,
      Properties::SmootherScale::Linear, 100.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Comb Filter", "Comb Filter", "Decay", "Decay",
      "Amount of the internal feedback noticeable as the decay time of the "
      "impules response [logarithmic scaling]. At negative values, the "
      "feedback signal is inverted, which shifts the fundamental resonance "
      "down by one octave."}},
    {None},
    {Parameters::Local_Unmodulateables::Comb_Flt_Decay_KT,
     PID::Comb_Flt_Decay_KT,
     0.33f,
     {Smoothers::Poly_Slow::Comb_Flt_Decay_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, None, None, "", "Comb Filter", "Comb Filter",
      "Decay - Key Trk", "Key Trk",
      "Key scaling of the decay time.\n0.0 %: equal time for all keys\n100.0 "
      "%: shortening to half time per octave, origin at C3 = 60 semitones"}},
    {None},
    {Parameters::Local_Modulateables::Comb_Flt_AP_Tune,
     PID::Comb_Flt_AP_Tune,
     1.0f,
     {Smoothers::Poly_Slow::Comb_Flt_AP_Tune, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 140.0f, 0.0f, false},
     {140, 1400, 140, 1400, "", "Comb Filter", "Comb Filter", "Allpass - Tune",
      "AP Tune",
      "Center frequency of the 2-pole allpass filter [in semitones]. It is the "
      "frequency where the phase is shifted by 180 degrees. The allpass is in "
      "series with the delay. At the maximum position (140 semitones), the "
      "allpass has no effect."}},
    {None},
    {Parameters::Local_Unmodulateables::Comb_Flt_AP_KT,
     PID::Comb_Flt_AP_KT,
     0.5f,
     {Smoothers::Poly_Slow::Comb_Flt_AP_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 2000, None, None, "", "Comb Filter", "Comb Filter",
      "Allpass - Key Trk", "Key Trk",
      "Key scaling of the allpass filter frequency.\n0.0 %: same tuning for "
      "all keys\n100.0 %: full tracking with the keys, origin at C3 = 60 "
      "semitones"}},
    {Parameters::Local_Modulateables::Comb_Flt_AP_Env_C,
     PID::Comb_Flt_AP_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_AP_Env_C, None,
      Properties::SmootherScale::Linear, 80.0f, 0.0f, true},
     {80, 800, 80, 800, "", "Comb Filter", "Comb Filter", "Allpass - Env C",
      "Env C",
      "Amount of modulation of the allpass center frequency by Envelope C [in "
      "semitones]."}},
    {Parameters::Local_Modulateables::Comb_Flt_AP_Res,
     PID::Comb_Flt_AP_Res,
     0.5f,
     {Smoothers::Poly_Slow::Comb_Flt_AP_Res,
      Signals::Quasipoly_Signals::Comb_Flt_AP_Res,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "Allpass - Reson",
      "AP Reson",
      "Resonance of the 2-pole allpass. Determines how much the phase shift "
      "increases around the center frequency."}},
    {None},
    {Parameters::Local_Modulateables::Comb_Flt_LP_Tune,
     PID::Comb_Flt_LP_Tune,
     1.0f,
     {Smoothers::Poly_Slow::Comb_Flt_LP_Tune, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 100.0f, 40.0f, false},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "Hi Cut",
      "Hi Cut",
      "Cutoff frequency [in semitones] of the lowpass filter that damps the "
      "higher frequencies in the comb filter's signal path."}},
    {None},
    {Parameters::Local_Unmodulateables::Comb_Flt_LP_KT,
     PID::Comb_Flt_LP_KT,
     0.5f,
     {Smoothers::Poly_Slow::Comb_Flt_LP_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 2000, None, None, "", "Comb Filter", "Comb Filter",
      "Hi Cut - Key Trk", "Key Trk",
      "Key scaling of the lowpass cutoff frequency.\n0.0 %: same cutoff for "
      "all keys\n100.0 %: full tracking with the keys, origin at C3 = 60 "
      "semitones"}},
    {Parameters::Local_Modulateables::Comb_Flt_LP_Env_C,
     PID::Comb_Flt_LP_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_LP_Env_C, None,
      Properties::SmootherScale::Linear, 100.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "Hi Cut - Env C ",
      "Env C",
      "Amount of modulation of the cutoff frequency of the lowpass filter by "
      "Envelope C [in semitones]."}},
    {Parameters::Local_Modulateables::Comb_Flt_PM,
     PID::Comb_Flt_PM,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_PM,
      Signals::Quasipoly_Signals::Comb_Flt_PM,
      Properties::SmootherScale::Parabolic, 0.9f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Comb Filter", "Comb Filter", "PM", "PM",
      "Amount of (phase) modulation of the comb filter pitch (delay time) by "
      "the output signals of Oscillator & Shaper A and B, with the PM A-B knob "
      "determining their mix."}},
    {None},
    {Parameters::Local_Modulateables::Comb_Flt_PM_A_B,
     PID::Comb_Flt_PM_A_B,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_PM_A_B,
      Signals::Quasipoly_Signals::Comb_Flt_PM_A_B,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "PM - A-B",
      "A - B",
      "The (phase) modulation signal for the Comb Filter as a crossfade "
      "between the outputs of Oscillator & Shaper A and Oscillator & Shaper "
      "B."}},
    {Parameters::Local_Modulateables::SV_Flt_In_A_B,
     PID::SV_Flt_In_A_B,
     0.0f,
     {Smoothers::Poly_Fast::SV_Flt_In_A_B,
      Signals::Quasipoly_Signals::SV_Flt_In_A_B,
      Properties::SmootherScale::Linear, -1.0f, 1.0f, false},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter", "A - B",
      "A - B",
      "The signal for the State Variable Filter as a crossfade between the "
      "outputs of Oscillator & Shaper A and Oscillator & Shaper B."}},
    {None},
    {Parameters::Local_Modulateables::SV_Flt_Comb_Mix,
     PID::SV_Flt_Comb_Mix,
     0.0f,
     {Smoothers::Poly_Fast::SV_Flt_Comb_Mix,
      Signals::Quasipoly_Signals::SV_Flt_Comb_Mix,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "State Variable Filter", "SV Filter",
      "Comb Mix", "Comb Mix",
      "The input signal for the State Variable Filter as mix of the signals "
      "from the A-B mixer and from the Comb Filter. Negative mix amounts will "
      "create different Comb Filter (cancellation) effects."}},
    {None},
    {Parameters::Local_Modulateables::SV_Flt_Cut,
     PID::SV_Flt_Cut,
     0.5f,
     {Smoothers::Poly_Slow::SV_Flt_Cut, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 120.0f, 20.0f, false},
     {120, 1200, 120, 1200, "", "State Variable Filter", "SV Filter", "Cutoff",
      "Cutoff",
      "Static value of the filter cutoff frequency at C3 [in semitones], "
      "applies to both stages of the filter. The offsets between their "
      "individual cutoffs is controlled by \"Spread\"."}},
    {None},
    {Parameters::Local_Unmodulateables::SV_Flt_Cut_KT,
     PID::SV_Flt_Cut_KT,
     0.5f,
     {Smoothers::Poly_Slow::SV_Flt_Cut_KT, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {200, 2000, None, None, "", "State Variable Filter", "SV Filter",
      "Cutoff - Key Trk", "Key Trk",
      "Key scaling of the filter cutoffs.\n0.0 %: no influence\n100.0 %: full "
      "tracking with the keys, origin at C3 = 60 semitones"}},
    {Parameters::Local_Modulateables::SV_Flt_Cut_Env_C,
     PID::SV_Flt_Cut_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::SV_Flt_Cut_Env_C, None,
      Properties::SmootherScale::Linear, 100.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "Cutoff - Env C", "Env C",
      "Amount of cutoff modulation by Envelope C [in semitones]."}},
    {Parameters::Local_Modulateables::SV_Flt_Res,
     PID::SV_Flt_Res,
     0.5f,
     {Smoothers::Poly_Slow::SV_Flt_Res, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "Resonance", "Reson",
      "Amount of filter resonance, creating peaks at the cutoff frequencies."}},
    {None},
    {Parameters::Local_Unmodulateables::SV_Flt_Res_KT,
     PID::SV_Flt_Res_KT,
     0.0f,
     {Smoothers::Poly_Slow::SV_Flt_Res_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "State Variable Filter", "SV Filter",
      "Reson - Key Trk", "Key Trk",
      "Key scaling of the filter resonance.\n0.0 %: no influence\n100.0 %: "
      "full tracking with the keys, origin at C3 = 60 semitones"}},
    {Parameters::Local_Modulateables::SV_Flt_Res_Env_C,
     PID::SV_Flt_Res_Env_C,
     0.0f,
     {Smoothers::Poly_Slow::SV_Flt_Res_Env_C, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "Reson - Env C ", "Env C",
      "Amount of resonance modulation by Envelope C."}},
    {Parameters::Local_Modulateables::SV_Flt_Spread,
     PID::SV_Flt_Spread,
     0.2f,
     {Smoothers::Poly_Slow::SV_Flt_Spread, None,
      Properties::SmootherScale::Linear, 30.0f, 0.0f, true},
     {60, 600, 60, 600, "", "State Variable Filter", "SV Filter", "Spread",
      "Spread",
      "Amount of splitting of the cutoffs of the two 2-pole filters. Half of "
      "the value is applied as a positive offset to the adjusted cutoff for "
      "the first stage and as a negative offset for the second stage [in "
      "semitones]. The split reduces the strong resonance peak in the 4-pole "
      "mode and allows filter curves with two formants. In bandpass/bandreject "
      "mode it controls the width of the band."}},
    {None},
    {Parameters::Local_Modulateables::SV_Flt_LBH,
     PID::SV_Flt_LBH,
     0.0f,
     {Smoothers::Poly_Fast::SV_Flt_LBH, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "L - B - H", "L-B-H",
      "Crossfades between the lowpass, bandpass and highpass outputs of the "
      "two filter stages.\n(first stage -> second stage: LP -> LP, BP -> LP, "
      "HP -> LP, HP -> BP, HP -> HP)"}},
    {None},
    {Parameters::Local_Modulateables::SV_Flt_Par,
     PID::SV_Flt_Par,
     0.0f,
     {Smoothers::Poly_Fast::SV_Flt_Par, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "Parallel", "Parallel ",
      "At zero, the two 2-pole filter stages are in series forming a 4-pole "
      "filter. Positive values crossfade from the serial structure to a "
      "parallel structure where the filter outputs are added. In bandpass mode "
      "(L-B-H = 50.0%), the parallel structure with a negative Spread works as "
      "a band-reject filter. Negative values also crossfade to a parallel "
      "structure, but here the lower filter is subtracted, which leads to "
      "phase cancellations."}},
    {Parameters::Local_Modulateables::SV_Flt_FM,
     PID::SV_Flt_FM,
     0.0f,
     {Smoothers::Poly_Slow::SV_Flt_FM, None, Properties::SmootherScale::Linear,
      5.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "State Variable Filter", "SV Filter", "FM",
      "FM",
      "Amount of modulation of the cutoff frequencies by the output signals of "
      "Oscillator & Shaper A and B, with the FM A-B knob determining their "
      "mix. The amount is relative to the cutoff frequency."}},
    {None},
    {Parameters::Local_Modulateables::SV_Flt_FM_A_B,
     PID::SV_Flt_FM_A_B,
     0.0f,
     {Smoothers::Poly_Slow::SV_Flt_FM_A_B,
      Signals::Quasipoly_Signals::SV_Flt_FM_A_B,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "State Variable Filter", "SV Filter",
      "FM - A-B", "A - B",
      "The signal for the FM (cutoff frequency modulation) of the State "
      "Variable Filter as a crossfade between the outputs of Oscillator & "
      "Shaper A and Oscillator & Shaper B."}},
    {Parameters::Local_Modulateables::FB_Mix_Comb,
     PID::FB_Mix_Comb,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_Comb,
      Signals::Quasipoly_Signals::FB_Mix_Comb,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Feedback Mixer", "FB Mixer", "Comb Filter",
      "Comb", "Feedback mix factor for the output of the Comb Filter."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_SVF,
     PID::FB_Mix_SVF,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_SVF, Signals::Quasipoly_Signals::FB_Mix_SVF,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Feedback Mixer", "FB Mixer", "SV Filter",
      "SV Filter",
      "Feedback mix factor for the output of the State Variable Filter."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_FX,
     PID::FB_Mix_FX,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_FX, Signals::Quasipoly_Signals::FB_Mix_FX,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Feedback Mixer", "FB Mixer", "Effects",
      "Effects",
      "Feedback mix factor for the output of the Effects chain. The reverb "
      "amount in the feedback can be set by the \"Reverb Amount\" fader "
      "independantly. Since the signal is monophonic, such feedback will cause "
      "intermodulation between the voices."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Rvb,
     PID::FB_Mix_Rvb,
     0.5f,
     {Smoothers::Poly_Fast::FB_Mix_Rvb, Signals::Quasipoly_Signals::FB_Mix_Rvb,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer", "Reverb Mix",
      "Rev Mix",
      "Controls the amount of reverb in the feedback independantly from the "
      "Mix in the Reverb section.\n0.0 %: dry, no reverb signal\n50.0 %: mix "
      "of 50 % dry and 50 % wet signal\n100.0 %: wet, 100.0 % reverb signal"}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Drive,
     PID::FB_Mix_Drive,
     2.0f / 7.0f,
     {Smoothers::Poly_Fast::FB_Mix_Drive,
      Signals::Quasipoly_Signals::FB_Mix_Drive,
      Properties::SmootherScale::Expon_Mix_Drive, 70.0f, 0.25f, false},
     {140, 700, 70, 700, "", "Feedback Mixer", "FB Mixer", "Drive", "Drive",
      "Input gain [in dB] of the sine shaper stage. Higher gains will create "
      "more distortion and harmonics."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Fold,
     PID::FB_Mix_Fold,
     0.5f,
     {Smoothers::Poly_Fast::FB_Mix_Fold,
      Signals::Quasipoly_Signals::FB_Mix_Fold,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer", "Fold", "Fold",
      "Amount of folding back of the shaper curve for high input "
      "amplitudes.\n0.0 %: flat saturation, no folding\n100.0 %: fully folded "
      "back (periodic sine curve)\nA higher amount of folding leads to a "
      "softer but more nasal sound."}},
    {Parameters::Local_Modulateables::FB_Mix_Asym,
     PID::FB_Mix_Asym,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_Asym,
      Signals::Quasipoly_Signals::FB_Mix_Asym,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer", "Asymmetry",
      "Asym",
      "Asymmetry of the shaper curve, generating even (2nd, 4th, ...) "
      "harmonics. At higher values, it becomes a parabolic curve that shifts "
      "the frequency of the fundamental to its double."}},
    {Parameters::Local_Unmodulateables::FB_Mix_Lvl_KT,
     PID::FB_Mix_Lvl_KT,
     0.0f,
     {Smoothers::Poly_Slow::FB_Mix_Lvl_KT, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, None, None, "", "Feedback Mixer", "FB Mixer",
      "Level - Key Trk", "Key Trk",
      "Key scaling of the feedback level [in dB per semitone].\npositive "
      "values: higher level for higher notes (+ 1.0 = + 12 dB per "
      "octave)\nnegative values: lower level for higher notes (- 1.0 = - 12 dB "
      "per octave)\norigin at C3 = 60 semitones"}},
    {Parameters::Local_Modulateables::Out_Mix_A_Lvl,
     PID::Out_Mix_A_Lvl,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_A_Lvl, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Output Mixer", "Output Mixer", "A - Level",
      "A", "Output mix factor for the signal from Oscillator & Shaper A."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_A_Pan,
     PID::Out_Mix_A_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_A_Pan, None,
      Properties::SmootherScale::Linear, 0.5f, 0.5f, true},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "A - Pan",
      "Pan",
      "Pan position of the signal from Oscillator & Shaper A in the output "
      "mix."}},
    {Parameters::Local_Modulateables::Out_Mix_B_Lvl,
     PID::Out_Mix_B_Lvl,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_B_Lvl, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Output Mixer", "Output Mixer", "B - Level",
      "B", "Output mix factor for the signal from Oscillator & Shaper B."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_B_Pan,
     PID::Out_Mix_B_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_B_Pan, None,
      Properties::SmootherScale::Linear, 0.5f, 0.5f, true},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "B - Pan",
      "Pan",
      "Pan position of the signal from Oscillator & Shaper B in the output "
      "mix."}},
    {Parameters::Local_Modulateables::Out_Mix_Comb_Lvl,
     PID::Out_Mix_Comb_Lvl,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_Comb_Lvl, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Output Mixer", "Output Mixer", "Comb - Level",
      "Comb", "Output mix factor for the signal from the Comb Filter."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_Comb_Pan,
     PID::Out_Mix_Comb_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_Comb_Pan, None,
      Properties::SmootherScale::Linear, 0.5f, 0.5f, true},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "Comb - Pan",
      "Pan",
      "Pan position of the signal from the Comb Filter in the output mix."}},
    {Parameters::Local_Modulateables::Out_Mix_SVF_Lvl,
     PID::Out_Mix_SVF_Lvl,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_SVF_Lvl, None,
      Properties::SmootherScale::Linear, 2.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Output Mixer", "Output Mixer",
      "SV Filter - Level", "SV Filter",
      "Output mix factor for the signal from the State Variable Filter."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_SVF_Pan,
     PID::Out_Mix_SVF_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_SVF_Pan, None,
      Properties::SmootherScale::Linear, 0.5f, 0.5f, true},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer",
      "SV Filter - Pan", "Pan",
      "Pan position of the signal from the State Variable Filter in the output "
      "mix."}},
    {Parameters::Local_Modulateables::Out_Mix_Drive,
     PID::Out_Mix_Drive,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_Drive,
      Signals::Quasipoly_Signals::Out_Mix_Drive,
      Properties::SmootherScale::Expon_Mix_Drive, 50.0f, 0.25f, false},
     {100, 500, 50, 500, "", "Output Mixer", "Output Mixer", "Drive", "Drive",
      "Input gain [in dB] of the sine shaper stage. Higher gains will create "
      "more distortion and harmonics."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_Fold,
     PID::Out_Mix_Fold,
     0.5f,
     {Smoothers::Poly_Fast::Out_Mix_Fold,
      Signals::Quasipoly_Signals::Out_Mix_Fold,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "Fold", "Fold",
      "Amount of folding back of the shaper curve for high input "
      "amplitudes.\n0.0 %: flat saturation, no folding\n100.0 %: fully folded "
      "back (periodic sine curve)\nA higher amount of folding leads to a "
      "softer but more nasal sound."}},
    {Parameters::Local_Modulateables::Out_Mix_Asym,
     PID::Out_Mix_Asym,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_Asym,
      Signals::Quasipoly_Signals::Out_Mix_Asym,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "Asymmetry",
      "Asym",
      "Asymmetry of the shaper curve, generating even (2nd, 4th, ...) "
      "harmonics. At higher values, it becomes a parabolic curve that shifts "
      "the frequency of the fundamental to its double."}},
    {Parameters::Local_Modulateables::Out_Mix_Lvl,
     PID::Out_Mix_Lvl,
     0.38f,
     {Smoothers::Poly_Fast::Out_Mix_Lvl,
      Signals::Quasipoly_Signals::Out_Mix_Lvl,
      Properties::SmootherScale::Parabolic, 2.56f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Output Mixer", "Output Mixer", "Level",
      "Level",
      "Master output level [in dB] of the synth. The output signal will be "
      "processed by the (monophonic) chain of effects."}},
    {None},
    {Parameters::Local_Unmodulateables::Out_Mix_Key_Pan,
     PID::Out_Mix_Key_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_Key_Pan, None,
      Properties::SmootherScale::Linear, 1.0f / 60.0f, 0.0f, true},
     {100, 1000, None, None, "", "Output Mixer", "Output Mixer", "Key Pan",
      "Key Pan",
      "Amount of key panning for each note (referring to the key position "
      "relative to the center at F#3 = 66 semitones). At zero, each note is "
      "centered before being passed to the panning section in the mix. At "
      "maximum, low notes will be panned to the left, high notes to the right. "
      "At minimum, the opposite  effect applies."}},
    {Parameters::Local_Modulateables::Cabinet_Drive,
     PID::Cabinet_Drive,
     0.4f,
     {Smoothers::Mono_Fast::Cabinet_Drive, Signals::Mono_Signals::Cabinet_Drive,
      Properties::SmootherScale::Expon_Gain, 50.0f, 0.0f, false},
     {100, 500, 50, 500, "", "Cabinet", "Cabinet", "Drive", "Drive",
      "Gain [in dB] for the input signal. Higher gains will increase the "
      "amount of distortion/saturation."}},
    {None},
    {Parameters::Local_Modulateables::Cabinet_Fold,
     PID::Cabinet_Fold,
     0.25f,
     {Smoothers::Mono_Slow::Cabinet_Fold, Signals::Mono_Signals::Cabinet_Fold,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Cabinet", "Cabinet", "Fold", "Fold",
      "Amount of folding back of the shaper curve for high input "
      "amplitudes.\n0.0 %: flat saturation, no folding\n100.0 %: fully folded "
      "back (periodic sine curve)\nA higher amount of folding leads to a "
      "softer but more nasal sound."}},
    {Parameters::Local_Modulateables::Cabinet_Asym,
     PID::Cabinet_Asym,
     0.25f,
     {Smoothers::Mono_Slow::Cabinet_Asym, Signals::Mono_Signals::Cabinet_Asym,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Cabinet", "Cabinet", "Asymmetry", "Asym",
      "Asymmetry of the shaper curve, generating even (2nd, 4th, ...) "
      "harmonics. At higher values, it becomes a parabolic curve that shifts "
      "the frequency of the fundamental to its double."}},
    {Parameters::Local_Modulateables::Cabinet_Tilt,
     PID::Cabinet_Tilt,
     -0.2f,
     {Smoothers::Mono_Fast::Cabinet_Tilt, None,
      Properties::SmootherScale::Linear, 100.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Cabinet", "Cabinet", "Tilt", "Tilt",
      "Controls two inverted shelving EQs before and after the distortion "
      "stage.\nNegative: more distortions at high frequencies\nPositive: more "
      "distortions at low frequencies"}},
    {None},
    {Parameters::Local_Modulateables::Cabinet_Hi_Cut,
     PID::Cabinet_Hi_Cut,
     50.0f / 80.0f,
     {Smoothers::Mono_Slow::Cabinet_Hi_Cut, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 80.0f, 60.0f, false},
     {80, 800, 80, 800, "", "Cabinet", "Cabinet", "Hi Cut", "Hi Cut",
      "Cutoff frequency [in semitones] of the lowpass filter at the output."}},
    {None},
    {Parameters::Local_Modulateables::Cabinet_Lo_Cut,
     PID::Cabinet_Lo_Cut,
     10.0f / 80.0f,
     {Smoothers::Mono_Slow::Cabinet_Lo_Cut, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 80.0f, 20.0f, false},
     {80, 800, 80, 800, "", "Cabinet", "Cabinet", "Lo Cut", "Lo Cut",
      "Cutoff frequency [in semitones] of the highpass filter at the input."}},
    {Parameters::Local_Modulateables::Cabinet_Cab_Lvl,
     PID::Cabinet_Cab_Lvl,
     36.0f / 50.0f,
     {Smoothers::Mono_Fast::Cabinet_Cab_Lvl, None,
      Properties::SmootherScale::Expon_Gain, 50.0f, -50.0f, false},
     {50, 500, 50, 500, "", "Cabinet", "Cabinet", "Cab Level", "Cab Level",
      "Output level [in dB] of the saturation effect before it is mixed with "
      "the dry signal."}},
    {None},
    {Parameters::Local_Modulateables::Cabinet_Mix,
     PID::Cabinet_Mix,
     0.0f,
     {Smoothers::Mono_Fast::Cabinet_Mix, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Cabinet", "Cabinet", "Mix", "Mix",
      "Crossfades between the dry signal and the saturated signal."}},
    {None},
    {Parameters::Local_Modulateables::Gap_Flt_Center,
     PID::Gap_Flt_Center,
     0.5f,
     {Smoothers::Mono_Slow::Gap_Flt_Center, None,
      Properties::SmootherScale::Linear, 96.0f, 24.0f, false},
     {96, 960, 96, 960, "", "Gap Filter", "Gap Filter", "Center", "Center",
      "Shifts the mean cutoff frequency of both 4-pole filters on both "
      "channels up or down [in semitones]."}},
    {None},
    {Parameters::Local_Modulateables::Gap_Flt_Stereo,
     PID::Gap_Flt_Stereo,
     0.0f,
     {Smoothers::Mono_Slow::Gap_Flt_Stereo, None,
      Properties::SmootherScale::Linear, 36.0f, 0.0f, true},
     {72, 720, 72, 720, "", "Gap Filter", "Gap Filter", "Stereo", "Stereo",
      "Sets the difference between the center frequencies of the left and of "
      "the right channel [in semitones]."}},
    {Parameters::Local_Modulateables::Gap_Flt_Gap,
     PID::Gap_Flt_Gap,
     12.0f / 96.0f,
     {Smoothers::Mono_Slow::Gap_Flt_Gap, None,
      Properties::SmootherScale::Linear, 48.0f, 0.0f, false},
     {96, 960, 96, 960, "", "Gap Filter", "Gap Filter", "Gap", "Gap",
      "Offset between the cutoffs of the lowpass and the highpass [in "
      "semitones]. When the filters are running in parallel (Mix > 0.0), the "
      "Gap determines the width of the attenuated band, in serial mode (Mix < "
      "0.0), the Gap determines the width of the passed band."}},
    {None},
    {Parameters::Local_Modulateables::Gap_Flt_Res,
     PID::Gap_Flt_Res,
     0.5f,
     {Smoothers::Mono_Slow::Gap_Flt_Res, Signals::Mono_Signals::Gap_Flt_Res,
      Properties::SmootherScale::Linear, 0.9f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Gap Filter", "Gap Filter", "Resonance",
      "Reson",
      "Resonance of both filters. Higher values create two resonance peaks at "
      "the upper and lower end of the gap."}},
    {Parameters::Local_Modulateables::Gap_Flt_Bal,
     PID::Gap_Flt_Bal,
     0.0f,
     {Smoothers::Mono_Fast::Gap_Flt_Bal, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Gap Filter", "Gap Filter", "Balance",
      "Balance",
      "Balance between the levels of the ranges above and below the gap.\nIn "
      "the center position both ranges are equally weighted.\nNegative values "
      "boost the lower range and attenuate the higher range, positive values "
      "have the opposite effect."}},
    {None},
    {Parameters::Local_Modulateables::Gap_Flt_Mix,
     PID::Gap_Flt_Mix,
     0.0f,
     {Smoothers::Mono_Fast::Gap_Flt_Mix, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Gap Filter", "Gap Filter", "Mix", "Mix",
      "Crossfades between the dry signal and the filtered signal. At positive "
      "values, the filter runs in parallel (band reject) mode, at negative "
      "values, it is a bandpass filter (in serial mode)."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Time_Mod,
     PID::Flanger_Time_Mod,
     0.0f,
     {Smoothers::Mono_Fast::Flanger_Time_Mod,
      Signals::Mono_Signals::Flanger_Time_Mod,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Flanger", "Flanger", "Time Mod", "Time Mod",
      "Relative amount of the modulation of the delay times by the LFO and/or "
      "the envelope."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Phase,
     PID::Flanger_Phase,
     0.5f,
     {Smoothers::Mono_Slow::Flanger_Phase, None,
      Properties::SmootherScale::Linear, 180.0f, 0.0f, false},
     {180, 1800, 180, 1800, "", "Flanger", "Flanger", "Phase", "Phase",
      "Phase offset between the LFO signals modulating the delay times in the "
      "left and the right channel."}},
    {Parameters::Local_Modulateables::Flanger_Rate,
     PID::Flanger_Rate,
     0.317f,
     {Smoothers::Mono_Slow::Flanger_Rate, None,
      Properties::SmootherScale::Parabolic, 10.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Flanger", "Flanger", "Rate", "Rate",
      "Frequency of the LFO and rate of the envelope. Both can be modulation "
      "sources for the delay times and the allpass center frequencies."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Time,
     PID::Flanger_Time,
     0.317f,
     {Smoothers::Mono_Slow::Flanger_Time, None,
      Properties::SmootherScale::Parabolic, 50.0f, 0.0f, false},
     {125, 1250, 125, 1250, "", "Flanger", "Flanger", "Time", "Time",
      "Mean value of the delay times in the left and right channel.\nThe "
      "\"Stereo\" parameter allows to create a time offset between the "
      "channels.\nWhen the time of the Flanger is set to zero, the overall "
      "effect is determined by the phase shifting of the allpass."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Stereo,
     PID::Flanger_Stereo,
     0.0f,
     {Smoothers::Mono_Slow::Flanger_Stereo, None,
      Properties::SmootherScale::Linear, 50.0f, 0.0f, true},
     {50, 500, 50, 500, "", "Flanger", "Flanger", "Stereo", "Stereo",
      "Sets the ratio between the delay times of the left and of the right "
      "channel [the value shows the offset to 100.0 %]. In the center "
      "position, the offset is zero and both delay times are equal."}},
    {Parameters::Local_Modulateables::Flanger_Feedback,
     PID::Flanger_Feedback,
     0.0f,
     {Smoothers::Mono_Fast::Flanger_Feedback, None,
      Properties::SmootherScale::Linear, 0.5f, 0.5f, true},
     {100, 1000, 100, 1000, "", "Flanger", "Flanger", "Feedback", "Feedback",
      "Amount of the internal feedback. At negative values, the feedback is "
      "inverted and will emphasize other frequencies than in the non-inverted "
      "mode."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Cross_FB,
     PID::Flanger_Cross_FB,
     0.5f,
     {Smoothers::Mono_Fast::Flanger_Cross_FB, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Flanger", "Flanger", "Cross FB", "Cross FB",
      "Amount of the cross feedback between the left and the right channel, "
      "increasing the complexity of the resulting signal. At negative values, "
      "the cross feedback is inverted and will emphasize other frequencies "
      "than in the non-inverted mode.\n0.0 %: no cross feedback\n+/- 100.0 %: "
      "all feedback is cross feedback"}},
    {Parameters::Local_Modulateables::Flanger_Hi_Cut,
     PID::Flanger_Hi_Cut,
     0.75f,
     {Smoothers::Mono_Slow::Flanger_Hi_Cut, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 80.0f, 60.0f, false},
     {80, 800, 80, 800, "", "Flanger", "Flanger", "Hi Cut", "Hi Cut",
      "Cutoff frequency [in semitones] of the filter that damps the higher "
      "frequencies of the delayed signal."}},
    {Parameters::Local_Modulateables::Flanger_Mix,
     PID::Flanger_Mix,
     0.0f,
     {Smoothers::Mono_Fast::Flanger_Mix, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Flanger", "Flanger", "Mix", "Mix",
      "Crossfades between the dry signal and the delayed signal. At negative "
      "values, the delayed signal is inverted."}},
    {None},
    {Parameters::Local_Modulateables::Echo_Time,
     PID::Echo_Time,
     0.433f,
     {Smoothers::Mono_Slow::Echo_Time, None,
      Properties::SmootherScale::Parabolic, 2.0f, 0.0f, false},
     {200, 2000, 200, 2000, "", "Echo", "Echo", "Time", "Time",
      "Mean delay time [in milliseconds]. (As there can be an offset between "
      "the left and right channel, this control shows the mean time.)"}},
    {None},
    {Parameters::Local_Modulateables::Echo_Stereo,
     PID::Echo_Stereo,
     0.0f,
     {Smoothers::Mono_Slow::Echo_Stereo, None,
      Properties::SmootherScale::Linear, 33.0f, 0.0f, true},
     {66, 660, 66, 660, "", "Echo", "Echo", "Stereo", "Stereo",
      "Sets the ratio between the delay times of the left and of the right "
      "channel [the value shows the offset to 100.0 %]. In the center "
      "position, the offset is zero and both delay times are equal."}},
    {None},
    {Parameters::Local_Modulateables::Echo_Feedback,
     PID::Echo_Feedback,
     0.5f,
     {Smoothers::Mono_Fast::Echo_Feedback, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Echo", "Echo", "Feedback", "Feedback",
      "Amount of internal feedback."}},
    {None},
    {Parameters::Local_Modulateables::Echo_Cross_FB,
     PID::Echo_Cross_FB,
     0.5f,
     {Smoothers::Mono_Fast::Echo_Cross_FB, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Echo", "Echo", "Cross FB", "Cross FB",
      "Amount of the cross feedback between the left and right channel.\n0.0 "
      "%: no cross feedback\n100.0 %: all feedback is cross feedback"}},
    {Parameters::Local_Modulateables::Echo_Hi_Cut,
     PID::Echo_Hi_Cut,
     0.75f,
     {Smoothers::Mono_Slow::Echo_Hi_Cut, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 80.0f, 60.0f, false},
     {80, 800, 80, 800, "", "Echo", "Echo", "Hi Cut", "Hi Cut",
      "Cutoff frequency [in semitones] of the filter that damps the higher "
      "frequencies of the delayed signal."}},
    {Parameters::Local_Modulateables::Echo_Mix,
     PID::Echo_Mix,
     0.0f,
     {Smoothers::Mono_Fast::Echo_Mix, None,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Echo", "Echo", "Mix", "Mix",
      "Crossfades between the dry signal and the delayed signal."}},
    {None},
    {Parameters::Local_Modulateables::Reverb_Size,
     PID::Reverb_Size,
     0.33f,
     {Smoothers::Mono_Slow::Reverb_Size, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Reverb", "Reverb", "Size", "Size",
      "The room size and reverb time are set here."}},
    {None},
    {Parameters::Local_Modulateables::Reverb_Pre_Dly,
     PID::Reverb_Pre_Dly,
     0.33f,
     {Smoothers::Mono_Slow::Reverb_Pre_Dly, None,
      Properties::SmootherScale::Cubic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Reverb", "Reverb", "Pre Delay", "Pre Delay",
      "Pre delay time, shifting the late reflections. This has a profound "
      "effect on the perceived room size."}},
    {Parameters::Local_Modulateables::Reverb_Color,
     PID::Reverb_Color,
     0.5f,
     {Smoothers::Mono_Slow::Reverb_Color, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Reverb", "Reverb", "Color", "Color",
      "Cutoff of the filter that damps the lower frequencies of the "
      "reverberation signal."}},
    {None},
    {Parameters::Local_Modulateables::Reverb_Chorus,
     PID::Reverb_Chorus,
     0.25f,
     {Smoothers::Mono_Slow::Reverb_Chorus, Signals::Mono_Signals::Reverb_Chorus,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Reverb", "Reverb", "Chorus", "Chorus",
      "This controls the internal modulation of the reverberation delays. At "
      "higher amounts, there is more movement and the diffusion is smoother. "
      "At lower amounts, the reverb becomes more static and metallic."}},
    {Parameters::Local_Modulateables::Reverb_Mix,
     PID::Reverb_Mix,
     0.0f,
     {Smoothers::Mono_Fast::Reverb_Mix, None,
      Properties::SmootherScale::Parabolic, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Reverb", "Reverb", "Mix", "Mix",
      "Crossfades between the dry signal and the reverberation signal."}},
    {None},
    {Parameters::Macro_Controls::MC_A,
     PID::MC_A,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:A",
      "@MC:A", ""}},
    {Parameters::Macro_Controls::MC_B,
     PID::MC_B,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:B",
      "@MC:B", ""}},
    {Parameters::Macro_Controls::MC_C,
     PID::MC_C,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:C",
      "@MC:C", ""}},
    {Parameters::Macro_Controls::MC_D,
     PID::MC_D,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:D",
      "@MC:D", ""}},
    {Parameters::Global_Modulateables::Master_Volume,
     PID::Master_Volume,
     0.5f,
     {Smoothers::Global_Fast::Master_Volume, None,
      Properties::SmootherScale::Parabolic, 4.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Master", "Master", "Master Volume", "Volume",
      "Master volume [in dB], applied at the end of the effect chain, before "
      "the soft clipper."}},
    {Parameters::Global_Modulateables::Master_Tune,
     PID::Master_Tune,
     0.0f,
     {Smoothers::Global_Slow::Master_Tune, Signals::Global_Signals::Master_Tune,
      Properties::SmootherScale::Linear, 48.0f, 0.0f, true},
     {48, 4800, 48, 4800, "", "Master", "Master", "Master Tune", "Tune",
      "Global pitch transposition [in semitones]."}},
    {Parameters::Local_Unmodulateables::Unison_Voices,
     PID::Unison_Voices,
     0.0f,
     {None, None, Properties::SmootherScale::Linear, 23.0f, 1.0f, false},
     {23, 23, None, None, "0", "Unison", "Unison", "Voices", "Voices",
      "Number of unison voices that will be assigned to a key. At 1, the "
      "unison effect is disabled."}},
    {Parameters::Local_Modulateables::Unison_Detune,
     PID::Unison_Detune,
     50.0f / 24000.0f,
     {Smoothers::Poly_Slow::Unison_Detune, None,
      Properties::SmootherScale::Linear, 24.0f, 0.0f, false},
     {240, 24000, 240, 24000, "", "Unison", "Unison", "Detune", "Detune",
      "Spreading of the pitches of the unison voices [in semitones]. The Fine "
      "mode allows for adjustments in steps of 0.001 semitones (0.1 cents)."}},
    {None},
    {Parameters::Local_Modulateables::Unison_Phase,
     PID::Unison_Phase,
     0.0f,
     {Smoothers::Poly_Audio::Unison_Phase, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {180, 3600, 180, 3600, "", "Unison", "Unison", "Phase", "Phase",
      "Spreading the (Oscillators') start phases of the unison voices to get "
      "different phase cancellations at the beginning of a note. The value is "
      "the phase range [in degrees] covered by a group of unison voices."}},
    {Parameters::Local_Modulateables::Unison_Pan,
     PID::Unison_Pan,
     0.0f,
     {Smoothers::Poly_Fast::Unison_Pan, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Unison", "Unison", "Pan", "Pan",
      "Voice panning inside of a group of unison voices. At 100.0 %, the "
      "voices are spread over the full stereo base."}},
    {Parameters::Hardware_Sources::Pedal_1,
     PID::Pedal_1,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 1",
      "Pedal 1",
      "This control is the parameter representation of the \"Pedal 1\" "
      "Hardware Source. If a pedal is connected, it will directly follow the "
      "external control. If the source's return behavior is set to \"Center\", "
      "the parameter will be bipolar."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_A,
     PID::Pedal_1_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:A", "Ped 1 to @MC:A",
      "Determines the influence of Pedal 1 on Macro Control A. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_B,
     PID::Pedal_1_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:B", "Ped 1 to @MC:B",
      "Determines the influence of Pedal 1 on Macro Control B. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_C,
     PID::Pedal_1_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:C", "Ped 1 to @MC:C",
      "Determines the influence of Pedal 1 on Macro Control C. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_D,
     PID::Pedal_1_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:D", "Ped 1 to @MC:D",
      "Determines the influence of Pedal 1 on Macro Control D. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Sources::Pedal_2,
     PID::Pedal_2,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 2",
      "Pedal 2",
      "This control is the parameter representation of the \"Pedal 2\" "
      "Hardware Source. If a pedal is connected, it will directly follow the "
      "external control. If the source's return behavior is set to \"Center\", "
      "the parameter will be bipolar."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_A,
     PID::Pedal_2_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:A", "Ped 2 to @MC:A",
      "Determines the influence of Pedal 2 on Macro Control A. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_B,
     PID::Pedal_2_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:B", "Ped 2 to @MC:B",
      "Determines the influence of Pedal 2 on Macro Control B. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_C,
     PID::Pedal_2_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:C", "Ped 2 to @MC:C",
      "Determines the influence of Pedal 2 on Macro Control C. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_D,
     PID::Pedal_2_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:D", "Ped 2 to @MC:D",
      "Determines the influence of Pedal 2 on Macro Control D. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Sources::Pedal_3,
     PID::Pedal_3,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 3",
      "Pedal 3",
      "This control is the parameter representation of the \"Pedal 3\" "
      "Hardware Source. If a pedal is connected, it will directly follow the "
      "external control. If the source's return behavior is set to \"Center\", "
      "the parameter will be bipolar."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_A,
     PID::Pedal_3_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:A", "Ped 3 to @MC:A",
      "Determines the influence of Pedal 3 on Macro Control A. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_B,
     PID::Pedal_3_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:B", "Ped 3 to @MC:B",
      "Determines the influence of Pedal 3 on Macro Control B. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_C,
     PID::Pedal_3_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:C", "Ped 3 to @MC:C",
      "Determines the influence of Pedal 3 on Macro Control C. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_D,
     PID::Pedal_3_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:D", "Ped 3 to @MC:D",
      "Determines the influence of Pedal 3 on Macro Control D. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Sources::Pedal_4,
     PID::Pedal_4,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 4",
      "Pedal 4",
      "This control is the parameter representation of the \"Pedal 4\" "
      "Hardware Source. If a pedal is connected, it will directly follow the "
      "external control. If the source's return behavior is set to \"Center\", "
      "the parameter will be bipolar."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_A,
     PID::Pedal_4_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:A", "Ped 4 to @MC:A",
      "Determines the influence of Pedal 4 on Macro Control A. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_B,
     PID::Pedal_4_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:B", "Ped 4 to @MC:B",
      "Determines the influence of Pedal 4 on Macro Control B. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_C,
     PID::Pedal_4_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:C", "Ped 4 to @MC:C",
      "Determines the influence of Pedal 4 on Macro Control C. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_D,
     PID::Pedal_4_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:D", "Ped 4 to @MC:D",
      "Determines the influence of Pedal 4 on Macro Control D. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Sources::Bender,
     PID::Bender,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Bender",
      "Bender",
      "This control is the parameter representation of the \"Bender\" Hardware "
      "Source. It will directly follow the Bender."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_A,
     PID::Bender_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:A", "Bend to @MC:A",
      "Determines the influence of the Bender on Macro Control A. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_B,
     PID::Bender_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:B", "Bend to @MC:B",
      "Determines the influence of the Bender on Macro Control B. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_C,
     PID::Bender_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:C", "Bend to @MC:C",
      "Determines the influence of the Bender on Macro Control C. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_D,
     PID::Bender_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:D", "Bend to @MC:D",
      "Determines the influence of the Bender on Macro Control D. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Sources::Aftertouch,
     PID::Aftertouch,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Aftertouch",
      "Aftertouch",
      "This control is the parameter representation of the \"Aftertouch\" "
      "Hardware Source. It will directly follow aftertouch gestures."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_A,
     PID::Aftertouch_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:A", "AT to @MC:A",
      "Determines the influence of the Aftertouch on Macro Control A. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_B,
     PID::Aftertouch_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:B", "AT to @MC:B",
      "Determines the influence of the Aftertouch on Macro Control B. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_C,
     PID::Aftertouch_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:C", "AT to @MC:C",
      "Determines the influence of the Aftertouch on Macro Control C. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_D,
     PID::Aftertouch_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:D", "AT to @MC:D",
      "Determines the influence of the Aftertouch on Macro Control D. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Sources::Ribbon_1,
     PID::Ribbon_1,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Ribbon 1",
      "Ribbon 1",
      "This control is the parameter representation of the \"Ribbon 1\" "
      "Hardware Source. It will directly follow the Ribbon and vice versa. If "
      "the source's return behavior is set to \"Center\", the parameter will "
      "be bipolar."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_A,
     PID::Ribbon_1_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:A", "Rib 1 to @MC:A",
      "Determines the influence of Ribbon 1 on Macro Control A. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_B,
     PID::Ribbon_1_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:B", "Rib 1 to @MC:B",
      "Determines the influence of Ribbon 1 on Macro Control B. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_C,
     PID::Ribbon_1_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:C", "Rib 1 to @MC:C",
      "Determines the influence of Ribbon 1 on Macro Control C. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_D,
     PID::Ribbon_1_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:D", "Rib 1 to @MC:D",
      "Determines the influence of Ribbon 1 on Macro Control D. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Sources::Ribbon_2,
     PID::Ribbon_2,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Ribbon 2",
      "Ribbon 2",
      "This control is the parameter representation of the \"Ribbon 2\" "
      "Hardware Source. It will directly follow the Ribbon and vice versa. If "
      "the source's return behavior is set to \"Center\", the parameter will "
      "be bipolar."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_A,
     PID::Ribbon_2_to_MC_A,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:A", "Rib 2 to @MC:A",
      "Determines the influence of Ribbon 2 on Macro Control A. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_B,
     PID::Ribbon_2_to_MC_B,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:B", "Rib 2 to @MC:B",
      "Determines the influence of Ribbon 2 on Macro Control B. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_C,
     PID::Ribbon_2_to_MC_C,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:C", "Rib 2 to @MC:C",
      "Determines the influence of Ribbon 2 on Macro Control C. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_D,
     PID::Ribbon_2_to_MC_D,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:D", "Rib 2 to @MC:D",
      "Determines the influence of Ribbon 2 on Macro Control D. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Local_Modulateables::Env_A_Att_Curve,
     PID::Env_A_Att_Curve,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Att_Curve, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Attack Curve",
      "Curve",
      "Curvature of the (polynomial) Attack segment. (negative: decelerating "
      "slope, zero: linear, positive: accelerating slope)"}},
    {Parameters::Local_Modulateables::Env_B_Att_Curve,
     PID::Env_B_Att_Curve,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Att_Curve, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Attack Curve",
      "Curve",
      "Curvature of the (polynomial) Attack segment. (negative: decelerating "
      "slope, zero: linear, positive: accelerating slope)"}},
    {Parameters::Local_Modulateables::Env_C_Att_Curve,
     PID::Env_C_Att_Curve,
     0.0f,
     {Smoothers::Poly_Sync::Env_C_Att_Curve, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope C", "Envelope C", "Attack Curve",
      "Curve",
      "Curvature of the (polynomial) Attack segment. (negative: decelerating "
      "slope, zero: linear, positive: accelerating slope)"}},
    {Parameters::Local_Modulateables::Env_C_Sus,
     PID::Env_C_Sus,
     0.0f,
     {Smoothers::Poly_Fast::Env_C_Sus, None, Properties::SmootherScale::Linear,
      1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Envelope C", "Envelope C", "Sustain Level",
      "Sustain",
      "Sustain level (bipolar, target of the second Decay segment)."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Lvl,
     PID::FB_Mix_Lvl,
     0.38f,
     {Smoothers::Poly_Fast::FB_Mix_Lvl, None,
      Properties::SmootherScale::Parabolic, 4.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer", "Level", "Level",
      "Level of the (global) Feedback mix [in dB]."}},
    {None},
    {Parameters::Local_Modulateables::Osc_A_Phase,
     PID::Osc_A_Phase,
     0.0f,
     {Smoothers::Poly_Audio::Osc_A_Phase,
      Signals::Quasipoly_Signals::Osc_A_Phase,
      Properties::SmootherScale::Linear, 0.5f, 0.0f, true},
     {90, 1800, 90, 1800, "0", "Oscillator A", "Oscillator A", "Phase", "Phase",
      "Start phase [in degrees]. The Oscillator will be set to this phase "
      "position with each Note-On."}},
    {Parameters::Local_Modulateables::Osc_B_Phase,
     PID::Osc_B_Phase,
     0.0f,
     {Smoothers::Poly_Audio::Osc_B_Phase,
      Signals::Quasipoly_Signals::Osc_B_Phase,
      Properties::SmootherScale::Linear, 0.5f, 0.0f, true},
     {90, 1800, 90, 1800, "0", "Oscillator B", "Oscillator B", "Phase", "Phase",
      "Start phase [in degrees]. The Oscillator will be set to this phase "
      "position with each Note-On."}},
    {Parameters::Local_Modulateables::Osc_A_Chirp,
     PID::Osc_A_Chirp,
     40.0f / 60.0f,
     {Smoothers::Poly_Slow::Osc_A_Chirp, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 60.0f, 80.0f, false},
     {60, 600, 60, 600, "", "Oscillator A", "Oscillator A", "Chirp", "Chirp",
      "Cutoff [in semitones] of the lowpass in the phase modulation signal "
      "path that can be applied to reduce the level and frequency of "
      "\"chirping\" appearing at higher amounts of self modulation, "
      "cross-feedback through the other oscillator, or global feedback."}},
    {Parameters::Local_Modulateables::Osc_B_Chirp,
     PID::Osc_B_Chirp,
     40.0f / 60.0f,
     {Smoothers::Poly_Slow::Osc_B_Chirp, None,
      Properties::SmootherScale::Expon_Lin_Pitch, 60.0f, 80.0f, false},
     {60, 600, 60, 600, "", "Oscillator B", "Oscillator B", "Chirp", "Chirp",
      "Cutoff [in semitones] of the lowpass in the phase modulation signal "
      "path that can be applied to reduce the level and frequency of "
      "\"chirping\" appearing at higher amounts of self modulation, "
      "cross-feedback through the other oscillator, or global feedback."}},
    {Parameters::Local_Modulateables::Comb_Flt_Decay_Gate,
     PID::Comb_Flt_Decay_Gate,
     0.0f,
     {Smoothers::Poly_Slow::Comb_Flt_Decay_Gate, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Comb Filter", "Comb Filter", "Decay - Gate",
      "Gate",
      "Amount of gating applied to the decay time. It reduces the decay time "
      "when the key is released (Note-off).\n0.0 % same decay time in the "
      "release phase\n100.0 %: the decay time is set to zero at the release of "
      "the key"}},
    {None},
    {Parameters::Local_Modulateables::Flanger_Envelope,
     PID::Flanger_Envelope,
     0.0f,
     {Smoothers::Mono_Slow::Flanger_Envelope, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Flanger", "Flanger", "Envelope", "Envelope",
      "Crossfades between the signals of the LFO and the envelope as "
      "modulation sources for the delay times and the allpass center "
      "frequencies."}},
    {Parameters::Local_Modulateables::Flanger_AP_Mod,
     PID::Flanger_AP_Mod,
     0.0f,
     {Smoothers::Mono_Slow::Flanger_AP_Mod, None,
      Properties::SmootherScale::Linear, 70.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Flanger", "Flanger", "Allpass Mod", "AP Mod",
      "Relative amount of the modulation of the allpass center frequencies by "
      "the LFO and/or the envelope. The allpass creates the effect of a "
      "phaser."}},
    {None},
    {Parameters::Local_Modulateables::Flanger_AP_Tune,
     PID::Flanger_AP_Tune,
     1.0f,
     {Smoothers::Mono_Slow::Flanger_AP_Tune, None,
      Properties::SmootherScale::Linear, 140.0f, 0.0f, false},
     {140, 1400, 140, 1400, "", "Flanger", "Flanger", "Allpass Tune", "AP Tune",
      "Mean center frequency of the 4-pole allpass filters which are in series "
      "with the delays. Their frequency-dependant phase shifts can create a "
      "\"Phaser\" effect. The phase shift is minimized by setting this control "
      "to its maximum.\nWhen the time of the flanger is set to zero, the "
      "allpass becomes the dominant part."}},
    {None},
    {Parameters::Global_Unmodulateables::Scale_Base_Key,
     PID::Scale_Base_Key,
     0.0f,
     {Smoothers::Global_Slow::Scale_Base_Key, None,
      Properties::SmootherScale::Linear, 11.0f, 0.0f, false},
     {12, 12, None, None, "", "Scale", "Scale", "Base Key", "Base Key",
      "Sets the base key for the custom scale. The scale is defined for the "
      "eleven keys above the base key and will be applied to all octaves "
      "accordingly."}},
    {Parameters::Global_Modulateables::Scale_Offset_1,
     PID::Scale_Offset_1,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_1,
      Signals::Global_Signals::Scale_Offset_1,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +1", "Offset +1",
      "Offset of the first key following the base key [in cents]. At zero, the "
      "interval to the base key would be the minor second of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_2,
     PID::Scale_Offset_2,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_2,
      Signals::Global_Signals::Scale_Offset_2,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +2", "Offset +2",
      "Offset of the second key following the base key [in cents]. At zero, "
      "the interval to the base key would be the major second of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_3,
     PID::Scale_Offset_3,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_3,
      Signals::Global_Signals::Scale_Offset_3,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +3", "Offset +3",
      "Offset of the third key following the base key [in cents]. At zero, the "
      "interval to the base key would be the minor third of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_4,
     PID::Scale_Offset_4,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_4,
      Signals::Global_Signals::Scale_Offset_4,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +4", "Offset +4",
      "Offset of the fourth key following the base key [in cents]. At zero, "
      "the interval to the base key would be the major third of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_5,
     PID::Scale_Offset_5,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_5,
      Signals::Global_Signals::Scale_Offset_5,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +5", "Offset +5",
      "Offset of the fifth key following the base key [in cents]. At zero, the "
      "interval to the base key would be the fourth of the equally tempered "
      "scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_6,
     PID::Scale_Offset_6,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_6,
      Signals::Global_Signals::Scale_Offset_6,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +6", "Offset +6",
      "Offset of the sixth key following the base key [in cents]. At zero, the "
      "interval to the base key would be the tritone of the equally tempered "
      "scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_7,
     PID::Scale_Offset_7,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_7,
      Signals::Global_Signals::Scale_Offset_7,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +7", "Offset +7",
      "Offset of the seventh key following the base key [in cents]. At "
      "zero,the interval to the base key would be the fifth of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_8,
     PID::Scale_Offset_8,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_8,
      Signals::Global_Signals::Scale_Offset_8,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +8", "Offset +8",
      "Offset of the eighth key following the base key [in cents]. At zero, "
      "the interval to the base key would be the minor sixth of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_9,
     PID::Scale_Offset_9,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_9,
      Signals::Global_Signals::Scale_Offset_9,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +9", "Offset +9",
      "Offset of the ninth key following the base key [in cents]. At zero, the "
      "interval to the base key would be the major sixth of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_10,
     PID::Scale_Offset_10,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_10,
      Signals::Global_Signals::Scale_Offset_10,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +10",
      "Offset +10",
      "Offset of the tenth key following the base key [in cents]. At zero, the "
      "interval to the base key would be the minor seventh of the equally "
      "tempered scale."}},
    {Parameters::Global_Modulateables::Scale_Offset_11,
     PID::Scale_Offset_11,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_11,
      Signals::Global_Signals::Scale_Offset_11,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +11",
      "Offset +11",
      "Offset of the eleventh key following the base key [in cents]. At zero, "
      "the interval to the base key would be the major seventh of the equally "
      "tempered scale."}},
    {Parameters::Macro_Times::MC_Time_A,
     PID::MC_Time_A,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:A Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control A. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Macro_Times::MC_Time_B,
     PID::MC_Time_B,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:B Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control B. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Macro_Times::MC_Time_C,
     PID::MC_Time_C,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:C Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control C. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Macro_Times::MC_Time_D,
     PID::MC_Time_D,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:D Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control D. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Local_Unmodulateables::Env_A_Dec_1_Vel,
     PID::Env_A_Dec_1_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Dec_1_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope A", "Envelope A", "Decay 1 Velocity",
      "Velocity",
      "Velocity influence on the Decay 1 time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Decay 1 time by high Note-On velocities."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_A_Dec_2_Vel,
     PID::Env_A_Dec_2_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_A_Dec_2_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope A", "Envelope A", "Decay 2 Velocity",
      "Velocity",
      "Velocity influence on the Decay 2 time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Decay 2 time by high Note-On velocities."}},
    {None},
    {Parameters::Local_Modulateables::Env_A_Elevate,
     PID::Env_A_Elevate,
     0.0f,
     {Smoothers::Poly_Fast::Env_A_Elevate, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope A", "Envelope A", "Elevate T - A",
      "Elevate",
      "Transforms the breakpoints of the Envelope signal, becoming more and "
      "more a flat organ-like curve towards the control's edges. At negative "
      "values, the Timbre part will be affected, controlling Phase Modulations "
      "and Shaper Drives. At positive values, the Amplitude part will be "
      "affected."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_B_Dec_1_Vel,
     PID::Env_B_Dec_1_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Dec_1_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope B", "Envelope B", "Decay 1 Velocity",
      "Velocity",
      "Velocity influence on the Decay 1 time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Decay 1 time by high Note-On velocities."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_B_Dec_2_Vel,
     PID::Env_B_Dec_2_Vel,
     0.0f,
     {Smoothers::Poly_Sync::Env_B_Dec_2_Vel, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, true},
     {60, 600, None, None, "", "Envelope B", "Envelope B", "Decay 2 Velocity",
      "Velocity",
      "Velocity influence on the Decay 2 time. The value represents the "
      "logarithmic amount of the reduction (negative values) or gain (positive "
      "values) of the Decay 2 time by high Note-On velocities."}},
    {None},
    {Parameters::Local_Modulateables::Env_B_Elevate,
     PID::Env_B_Elevate,
     0.0f,
     {Smoothers::Poly_Fast::Env_B_Elevate, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Envelope B", "Envelope B", "Elevate T - A",
      "Elevate",
      "Transforms the breakpoints of the Envelope signal, becoming more and "
      "more a flat organ-like curve towards the control's edges. At negative "
      "values, the Timbre part will be affected, controlling Phase Modulations "
      "and Shaper Drives. At positive values, the Amplitude part will be "
      "affected."}},
    {None},
    {Parameters::Local_Unmodulateables::Env_C_Retr_H,
     PID::Env_C_Retr_H,
     0.0f,
     {Smoothers::Poly_Sync::Env_C_Retr_H, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, None, None, "", "Envelope C", "Envelope C", "Retrig Hardness",
      "Retrig",
      "Determines how much the Envelope signal is forced to start at zero "
      "instead of the current level (usually from the Release phase). At 100.0 "
      "% the Envelope always starts at zero, independant from the previous "
      "signal level."}},
    {None},
    {Parameters::Local_Modulateables::Echo_Send,
     PID::Echo_Send,
     1.0f,
     {Smoothers::Mono_Fast::Echo_Send, Signals::Mono_Signals::Echo_Send,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Echo", "Echo", "Send", "Send",
      "Level control at the input of the effect signal path. In contrast to "
      "the Echo Mix parameter, the Delay feedback will still be audible when "
      "setting the Send parameter to zero."}},
    {None},
    {Parameters::Local_Modulateables::Reverb_Send,
     PID::Reverb_Send,
     1.0f,
     {Smoothers::Mono_Fast::Reverb_Send, Signals::Mono_Signals::Reverb_Send,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Reverb", "Reverb", "Send", "Send",
      "Level control at the input of the effect signal path. In contrast to "
      "the Reverb Mix parameter, the Reverb feedback will still be audible "
      "when setting the Send parameter to zero."}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Osc,
     PID::FB_Mix_Osc,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_Osc, Signals::Quasipoly_Signals::FB_Mix_Osc,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "0", "Feedback Mixer", "FB Mixer",
      "A/B @ST:Between", "A/B @ST:Between",
      "Level of the signal from branch A or B of the other Part. (Only "
      "available when using Layer mode.)"}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Osc_Src,
     PID::FB_Mix_Osc_Src,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_Osc_Src,
      Signals::Quasipoly_Signals::FB_Mix_Osc_Src,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer", "A - B", "A - B",
      "Crossfades between branch A and B of the other Part. (Only available "
      "when using Layer mode.)"}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_Comb_Src,
     PID::FB_Mix_Comb_Src,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_Comb_Src,
      Signals::Quasipoly_Signals::FB_Mix_Comb_Src,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer",
      "Comb - From @VG:Other", "From @VG:Other",
      "Crossfades to the Comb Filter signal of the other Part. (Only available "
      "when using Layer mode.)"}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_SVF_Src,
     PID::FB_Mix_SVF_Src,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_SVF_Src,
      Signals::Quasipoly_Signals::FB_Mix_SVF_Src,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer",
      "SV Filter - From @VG:Other", "From @VG:Other",
      "Crossfades to the State Variable Filter signal of the other Part. (Only "
      "available when using Layer mode.)"}},
    {None},
    {Parameters::Local_Modulateables::FB_Mix_FX_Src,
     PID::FB_Mix_FX_Src,
     0.0f,
     {Smoothers::Poly_Fast::FB_Mix_FX_Src,
      Signals::Quasipoly_Signals::FB_Mix_FX_Src,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Feedback Mixer", "FB Mixer",
      "Effects - From @VG:Other", "From @VG:Other",
      "Crossfades to the Effects signal of the other Part. (Only available "
      "when using Layer mode.)"}},
    {None},
    {Parameters::Local_Modulateables::Split_Split_Point,
     PID::Split_Split_Point,
     30.0f / 60.0f,
     {None, None, Properties::SmootherScale::Linear, 60.0f, 0.05f, false},
     {60, 60, 60, 60, "", "Split", "Split", "Split Point", "Split Pnt",
      "Determines the split position of a sound as a key number. Keys from the "
      "lower end up to the Split Point will be associated to Part I, keys from "
      "the Split Point up to the upper end will be associated to Part II. When "
      "linked, both split positions are sticked together and the Split Point "
      "behaves as one global parameter applying to both Parts. When unlinked, "
      "the Split Point behaves as two local parameters which can be adjusted "
      "separately (any MC assignment is still shared, though), allowing for "
      "overlapping key ranges."}},
    {None},
    {Parameters::Local_Modulateables::Voice_Grp_Volume,
     PID::Voice_Grp_Volume,
     0.5f,
     {Smoothers::Poly_Fast::Voice_Grp_Volume, None,
      Properties::SmootherScale::Parabolic, 4.0f, 0.0f, false},
     {100, 1000, 100, 1000, "", "Voices", "Voices", "Part Volume", "Volume",
      "Part volume [in dB], available when using Split or Layer mode. Applied "
      "at the end of the corresponding effect chain, before the Master "
      "Volume."}},
    {None},
    {Parameters::Local_Modulateables::Voice_Grp_Tune,
     PID::Voice_Grp_Tune,
     0.0f,
     {Smoothers::Poly_Slow::Voice_Grp_Tune, None,
      Properties::SmootherScale::Linear, 48.0f, 0.0f, true},
     {48, 4800, 48, 4800, "", "Voices", "Voices", "Part Tune", "Tune",
      "Part pitch transposition [in semitones], available when using Split or "
      "Layer mode. Applied for the corresponding Part only."}},
    {None},
    {Parameters::Local_Modulateables::Out_Mix_To_FX,
     PID::Out_Mix_To_FX,
     0.0f,
     {Smoothers::Poly_Fast::Out_Mix_To_FX, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Output Mixer", "Output Mixer",
      "To FX @VG:Other", "To FX @VG:Other",
      "Determines how much of the Output Mixer signal is sent to the Effects "
      "section of the other Part. At 100.0 % the signal will be sent to the "
      "other Part exclusively. (Only available when using a Dual mode.)"}},
    {None},
    {Parameters::Local_Unmodulateables::Mono_Grp_Enable,
     PID::Mono_Grp_Enable,
     0.0f,
     {None, None, Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {1, 1, None, None, "0", "Mono", "Mono", "Enable", "Enable",
      "When enabled, only one voice will be assigned, even when multiple keys "
      "are pressed."}},
    {Parameters::Local_Unmodulateables::Mono_Grp_Prio,
     PID::Mono_Grp_Prio,
     0.0f,
     {None, None, Properties::SmootherScale::Linear, 2.0f, 0.0f, false},
     {2, 2, None, None, "", "Mono", "Mono", "Key Priority", "Priority",
      "When pressing multiple keys in Mono mode, they are sorted according to "
      "the priority set by this parameter. The key matching the priority "
      "determines the pitch of the Mono voice."}},
    {Parameters::Local_Unmodulateables::Mono_Grp_Legato,
     PID::Mono_Grp_Legato,
     1.0f,
     {None, None, Properties::SmootherScale::Linear, 3.0f, 0.0f, false},
     {3, 3, None, None, "0", "Mono", "Mono", "Legato", "Legato",
      "When a new key matches the Priority, it will take over. When the "
      "priorized key is released the second priorized key takes over. "
      "Envelopes will start if the state of this parameter allows for it. "
      "Glide will be applied if the state of this parameter allows for it."}},
    {Parameters::Local_Modulateables::Mono_Grp_Glide,
     PID::Mono_Grp_Glide,
     0.0f,
     {Smoothers::Poly_Slow::Mono_Grp_Glide, None,
      Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, false},
     {100, 1000, 100, 1000, "", "Mono", "Mono", "Glide Time", "Glide",
      "In Legato mode this time can be used to smooth the transitions of the "
      "pitches."}},
    {None},
    {Parameters::Macro_Controls::MC_E,
     PID::MC_E,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:E",
      "@MC:E", ""}},
    {Parameters::Macro_Times::MC_Time_E,
     PID::MC_Time_E,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:E Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control E. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Macro_Controls::MC_F,
     PID::MC_F,
     0.5f,
     {},
     {100, 1000, None, None, "", "Macro Control", "Macro Control", "@MC:F",
      "@MC:F", ""}},
    {Parameters::Macro_Times::MC_Time_F,
     PID::MC_Time_F,
     0.4f,
     {None, None, Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f,
      false},
     {100, 1000, None, None, "", "Macro Control", "Macro Control",
      "@MC:F Smoothing", "Smoothing",
      "Sets the smoothing time [in milliseconds] for modulations on target "
      "parameters assigned to Macro Control F. At zero, assigned target "
      "parameters will follow the Macro Control immediately."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_E,
     PID::Pedal_1_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:E", "Ped 1 to @MC:E",
      "Determines the influence of Pedal 1 on Macro Control E. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_1_to_MC_F,
     PID::Pedal_1_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 1 to @MC:F", "Ped 1 to @MC:F",
      "Determines the influence of Pedal 1 on Macro Control F. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_E,
     PID::Pedal_2_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:E", "Ped 2 to @MC:E",
      "Determines the influence of Pedal 2 on Macro Control E. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_2_to_MC_F,
     PID::Pedal_2_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 2 to @MC:F", "Ped 2 to @MC:F",
      "Determines the influence of Pedal 2 on Macro Control F. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_E,
     PID::Pedal_3_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:E", "Ped 3 to @MC:E",
      "Determines the influence of Pedal 3 on Macro Control E. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_3_to_MC_F,
     PID::Pedal_3_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 3 to @MC:F", "Ped 3 to @MC:F",
      "Determines the influence of Pedal 3 on Macro Control F. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_E,
     PID::Pedal_4_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:E", "Ped 4 to @MC:E",
      "Determines the influence of Pedal 4 on Macro Control E. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Pedal_4_to_MC_F,
     PID::Pedal_4_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Pedal 4 to @MC:F", "Ped 4 to @MC:F",
      "Determines the influence of Pedal 4 on Macro Control F. If the pedal is "
      "set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_E,
     PID::Bender_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:E", "Bend to @MC:E",
      "Determines the influence of the Bender on Macro Control E. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Amounts::Bender_to_MC_F,
     PID::Bender_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Bender to @MC:F", "Bend to @MC:F",
      "Determines the influence of the Bender on Macro Control F. The amount "
      "can be adjusted continuously [in percent] and can cover the whole range "
      "of the Macro Control in both directions (-100.0 ... 100.0 %). The "
      "bipolar range of the Bender is mapped to the unipolar range of the "
      "Macro Control by an amount of 50 %."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_E,
     PID::Aftertouch_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:E", "AT to @MC:E",
      "Determines the influence of the Aftertouch on Macro Control E. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Amounts::Aftertouch_to_MC_F,
     PID::Aftertouch_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Aftertouch to @MC:F", "AT to @MC:F",
      "Determines the influence of the Aftertouch on Macro Control F. The "
      "amount can be adjusted continuously [in percent] and can cover the "
      "whole range of the Macro Control in both directions (-100.0 ... 100.0 "
      "%)."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_E,
     PID::Ribbon_1_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:E", "Rib 1 to @MC:E",
      "Determines the influence of Ribbon 1 on Macro Control E. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_1_to_MC_F,
     PID::Ribbon_1_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 1 to @MC:F", "Rib 1 to @MC:F",
      "Determines the influence of Ribbon 1 on Macro Control F. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_E,
     PID::Ribbon_2_to_MC_E,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:E", "Rib 2 to @MC:E",
      "Determines the influence of Ribbon 2 on Macro Control E. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Hardware_Amounts::Ribbon_2_to_MC_F,
     PID::Ribbon_2_to_MC_F,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Amount", "HW Amount",
      "Ribbon 2 to @MC:F", "Rib 2 to @MC:F",
      "Determines the influence of Ribbon 2 on Macro Control F. If the ribbon "
      "is set to a returning mode, the amount can be adjusted continuously, "
      "otherwise it will be switch-like. Continuous amounts can be set [in "
      "percent] and can cover the whole range of the Macro Control in both "
      "directions (-100.0 ... 100.0 %)."}},
    {Parameters::Local_Modulateables::Flanger_Tremolo,
     PID::Flanger_Tremolo,
     0.0f,
     {Smoothers::Mono_Fast::Flanger_Tremolo,
      Signals::Mono_Signals::Flanger_Tremolo, Properties::SmootherScale::Linear,
      1.0f, 0.0f, false},
     {100, 1000, 100, 1000, "0", "Flanger", "Flanger", "Tremolo", "Tremolo",
      "Determines the amount of amplitude modulation by the Flanger's stereo "
      "LFO. It is applied after the Mix stage of the Flanger. The Rate and "
      "Phase parameters control the frequency and stereo spreading of this "
      "effect."}},
    {None},
    {Parameters::Global_Modulateables::Scale_Offset_0,
     PID::Scale_Offset_0,
     0.0f,
     {Smoothers::Global_Slow::Scale_Offset_0,
      Signals::Global_Signals::Scale_Offset_0,
      Properties::SmootherScale::Linear, 12.0f, 0.0f, true},
     {1200, 12000, 1200, 12000, "", "Scale", "Scale", "Offset +0", "Offset +0",
      "Offset of the base key [in cents]. At zero, the interval to the base "
      "key would be perfect identity."}},
    {None},
    {Parameters::Local_Unmodulateables::Osc_A_Reset,
     PID::Osc_A_Reset,
     1.0f,
     {Smoothers::Poly_Sync::Osc_A_Reset,
      Signals::Quasipoly_Signals::Osc_A_Reset,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {1, 1, None, None, "", "Oscillator A", "Oscillator A", "Reset", "Reset",
      "If enabled, the Oscillator will reset its phase everytime the envelopes "
      "start (in the corresponding voice). If disabled, the Oscillator is in "
      "free-running mode."}},
    {Parameters::Local_Unmodulateables::Osc_B_Reset,
     PID::Osc_B_Reset,
     1.0f,
     {Smoothers::Poly_Sync::Osc_B_Reset,
      Signals::Quasipoly_Signals::Osc_B_Reset,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, false},
     {1, 1, None, None, "", "Oscillator B", "Oscillator B", "Reset", "Reset",
      "If enabled, every pressed key will cause the Oscillator to reset its "
      "phase (for the corresponding voice). If disabled, the Oscillator is in "
      "free-running mode."}},
    {Parameters::Local_Unmodulateables::Voice_Grp_Mute,
     PID::Voice_Grp_Mute,
     0.0f,
     {Smoothers::Poly_Fast::Voice_Grp_Mute, None,
      Properties::SmootherScale::Linear, -1.0f, 1.0f, false},
     {1, 1, None, None, "", "Voices", "Voices", "Part Mute", "Part Mute",
      "Part mute, available when usig Split or Layer mode. When enabled, the "
      "corresponding Part will be muted."}},
    {Parameters::Local_Unmodulateables::Voice_Grp_Fade_From,
     PID::Voice_Grp_Fade_From,
     0.0f,
     {Smoothers::Poly_Sync::Voice_Grp_Fade_From, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, false},
     {60, 60, None, None, "", "Voices", "Voices", "Fade From", "Fade From",
      "Available when using Layer mode. For Part I the full level range spans "
      "from the lowest key (C1) to the key set by this parameter. This key "
      "defines the breakpoint where the fade-out begins. The fade-out width is "
      "adjustable by the \"Fade Range\" parameter. For Part II the full level "
      "range spans from the highest key (C6) to the key set here. For the keys "
      "below this breakpoint the level is faded out.\nBy default this "
      "parameter is set to the highest key (C6) for Part I and to the lowest "
      "key (C1) for Part II, so that both Parts cover the whole key range "
      "without fading."}},
    {Parameters::Local_Unmodulateables::Voice_Grp_Fade_Range,
     PID::Voice_Grp_Fade_Range,
     0.0f,
     {Smoothers::Poly_Sync::Voice_Grp_Fade_Range, None,
      Properties::SmootherScale::Linear, 60.0f, 0.0f, false},
     {60, 600, None, None, "", "Voices", "Voices", "Fade Range", "Fade Range",
      "Available when using Layer mode. This parameter adjusts the width of a "
      "key range [in semitones] where the level of a Part is faded out. For "
      "Part I the fade-out applies to keys above the breakpoint set by \"Fade "
      "From\". For Part II the fade-out applies to keys below the breakpoint "
      "set by \"Fade From\".\nWith a Fade Range of zero the Part is muted for "
      "all keys beyond the breakpoint, which is the default behavior. You can "
      "set this parameter in fractions of semitones to make subtle changes to "
      "the curve of the fade-out."}},
    {Parameters::Display_Parameters::Pedal_1_Send,
     PID::Pedal_1_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 1 Send",
      "Pedal 1 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Pedal 1\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Pedal 1\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Pedal 1\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Pedal_2_Send,
     PID::Pedal_2_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 2 Send",
      "Pedal 2 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Pedal 2\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Pedal 2\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Pedal 2\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Pedal_3_Send,
     PID::Pedal_3_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 3 Send",
      "Pedal 3 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Pedal 3\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Pedal 3\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Pedal 3\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Pedal_4_Send,
     PID::Pedal_4_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Pedal 4 Send",
      "Pedal 4 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Pedal 4\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Pedal 4\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Pedal 4\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Bender_Send,
     PID::Bender_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source", "Bender Send",
      "Bender Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Bender\" Hardware Source. It reflects "
      "changes that will be sent via MIDI without affecting the Synth. This "
      "parameter will be visible next to the internal representation of "
      "\"Bender\" (which reflects changes from the User Interface or MIDI "
      "Input, only affecting the Synth). Editing the actual \"Bender\" "
      "parameter will not be reflected."}},
    {Parameters::Display_Parameters::Aftertouch_Send,
     PID::Aftertouch_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source",
      "Aftertouch Send", "Aftertouch Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Aftertouch\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Aftertouch\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Aftertouch\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Ribbon_1_Send,
     PID::Ribbon_1_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source",
      "Ribbon 1 Send", "Ribbon 1 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Ribbon 1\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Ribbon 1\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Ribbon 1\" parameter will not be reflected."}},
    {Parameters::Display_Parameters::Ribbon_2_Send,
     PID::Ribbon_2_Send,
     0.0f,
     {},
     {100, 1000, None, None, "", "Hardware Source", "HW Source",
      "Ribbon 2 Send", "Ribbon 2 Snd",
      "When Local is disabled in the MIDI Settings, this control is the "
      "parameter representation of the \"Ribbon 2\" Hardware Source. It "
      "reflects changes that will be sent via MIDI without affecting the "
      "Synth. This parameter will be visible next to the internal "
      "representation of \"Ribbon 2\" (which reflects changes from the User "
      "Interface or MIDI Input, only affecting the Synth). Editing the actual "
      "\"Ribbon 2\" parameter will not be reflected."}},
    {Parameters::Global_Modulateables::Master_Pan,
     PID::Master_Pan,
     0.0f,
     {Smoothers::Global_Fast::Master_Pan, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Master", "Master", "@ST:Pan Pan",
      "@ST:Pan Pan",
      "Panning of the dual effect chain (not available in Single Sounds). The "
      "stereo signals of both  \"FX \" Parts can be panned against each "
      "other:\n-100%: Part I is panned completely to the right, Part II is "
      "panned completely to the left.\n0%: no additional panning, both Parts "
      "equally feed the resulting stereo signal.\n100%: Part I is panned "
      "completely to the left, Part II is panned completely to the right."}},
    {None},
    {Parameters::Global_Modulateables::Master_Serial_FX,
     PID::Master_Serial_FX,
     0.0f,
     {Smoothers::Global_Fast::Master_Serial_FX, None,
      Properties::SmootherScale::Linear, 1.0f, 0.0f, true},
     {100, 1000, 100, 1000, "", "Master", "Master", "Serial FX", "Serial FX",
      "The output of an FX chain can be routed to the input of the other FX "
      "chain, thus creating a series of 10 effects (not available in Single "
      "Sounds).\nNegative values: the FX II output signals are routed to FX I "
      "(with absolute amount)\n0.0%: no routing of signals between the FX "
      "chains\nPositive values: the FX I output signals are routed to FX II "
      "(with absolute amount)"}},
};

} // namespace C15
