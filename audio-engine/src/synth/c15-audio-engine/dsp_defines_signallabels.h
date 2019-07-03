/******************************************************************************/
/** @file       dsp_defines_signallabels.h
    @date       2018-08-21
    @version    1.0
    @author     Matthias Seeber & Anton Schmied
    @brief      Defines for the individual Signal Names (indices) in the great
                shared m_paramsignaldata[][] array
*******************************************************************************/

#pragma once

enum class Signals
{

  /* Envelope Signals                 -- polyphonic */
  Invalid = -1,
  Unused = 16383,

  ENV_A_MAG = 0,   // Envelope A (magnitude component)
  ENV_A_TMB = 1,   // Envelope A (timbre component)
  ENV_B_MAG = 2,   // Envelope B (magnitude component)
  ENV_B_TMB = 3,   // Envelope B (timbre component)
  ENV_C_CLIP = 4,  // Envelope C (clipped)
  ENV_C_UNCL = 5,  // Envelope C (unclipped)
  ENV_G_SIG = 6,   // Gate Envelope

  /* Oscillator A Signals             -- polyphonic */

  OSC_A_FRQ
  = 7,  // Oscillator A Frequency (Pitch, Pitch KT, Pitch Env C Amount, Env C, Master Tune, Reference, Key Pitch)
  OSC_A_FLUEC = 8,   // Oscillator A Fluctuation (Amount, Env C Amount, Env C)
  OSC_A_PMSEA = 9,   // Oscillator A PM Self (Amount, Env A Amount, Env A)
  OSC_A_PMSSH = 10,  // Oscillator A PM Self Shaper (Amount)
  OSC_A_PMBEB = 11,  // Oscillator A PM B (Amount, Env B Amount, Env B)
  OSC_A_PMBSH = 12,  // Oscillator A PM B Shaper (Amount)
  OSC_A_PMFEC = 13,  // Oscillator A PM FB (Amount, Env C Amount, Env C)
  OSC_A_PHS = 14,    // Oscillator A Phase (Offset)
  OSC_A_CHI = 15,    // Oscillator A Chirp Frequency (Cutoff)

  /* Shaper A Signals                 -- polyphonic */

  SHP_A_DRVEA = 16,  // Shaper A Drive (Drive, Env A Amount, Env A)
  SHP_A_FLD = 17,    // Shaper A Fold (Amount)
  SHP_A_ASM = 18,    // Shaper A Asymetry (Amount)
  SHP_A_MIX = 19,    // Shaper A Mix (Amount)
  SHP_A_FBM = 20,    // Shaper A Feedback Mix (Amount)
  SHP_A_FBEC = 21,   // Shaper A Feedback Env C (Amount, Env C, Gate)
  SHP_A_RM = 22,     // Shaper A Ringmod (Amount)

  /* Oscillator B Signals             -- polyphonic */

  OSC_B_FRQ
  = 23,  // Oscillator B Frequency (Pitch, Pitch KT, Pitch Env C Amount, Env C, Master Tune, Reference, Key Pitch)
  OSC_B_FLUEC = 24,  // Oscillator B Fluctuation (Amount, Env C Amount, Env C)
  OSC_B_PMSEB = 25,  // Oscillator B PM Self (Amount, Env B Amount, Env B)
  OSC_B_PMSSH = 26,  // Oscillator B PM Self Shaper (Amount)
  OSC_B_PMAEA = 27,  // Oscillator B PM A (Amount, Env A Amount, Env A)
  OSC_B_PMASH = 28,  // Oscillator B PM A Shaper (Amount)
  OSC_B_PMFEC = 29,  // Oscillator B PM FB (Amount, Env C Amount, Env C)
  OSC_B_PHS = 30,    // Oscillator B Phase (Offset)
  OSC_B_CHI = 31,    // Oscillator B Chirp Frequency (Cutoff)

  /* Shaper B Signals                 -- polyphonic */

  SHP_B_DRVEB = 32,  // Shaper B Drive (Drive, Env B Amount, Env B)
  SHP_B_FLD = 33,    // Shaper B Fold (Amount)
  SHP_B_ASM = 34,    // Shaper B Asymetry (Amount)
  SHP_B_MIX = 35,    // Shaper B Mix (Amount)
  SHP_B_FBM = 36,    // Shaper B Feedback Mix (Amount)
  SHP_B_FBEC = 37,   // Shaper B Feedback Env C (Amount, Env C, Gate)
  SHP_B_RM = 38,     // Shaper B Ringmod (Amount)

  /* Comb Filter Signals              -- polyphonic */

  CMB_AB = 39,    // Comb AB Input Mix (Amount)
  CMB_FRQ = 40,   // Comb Frequency (Pitch, Pitch KT, Master Tune, Reference, Key Pitch)
  CMB_BYP = 41,   // Comb Bypass (Pitch)
  CMB_FEC = 42,   // Comb Pitch Env C (Frequency Factor)
  CMB_DEC = 43,   // Comb Decay (Decay, Decay KT, Decay Gate Amount, Gate, Master Tune, Key Pitch)
  CMB_APF = 44,   // Comb Allpass Frequency (Tune, AP KT, AP Env C Amount, Env C, Master Tune, Key Pitch)
  CMB_APR = 45,   // Comb Allpass Resonance (Amount)
  CMB_LPF = 46,   // Comb Hi Cut Frequency (Hi Cut, Hi Cut KT, Hi Cut Env C, Env C, Master Tune, Key Pitch)
  CMB_PM = 47,    // Comb PM (Amount)
  CMB_PMAB = 48,  // Comb PM AB Mix (Amount)

  /* State Variable Filter Signals    -- polyphonic */

  SVF_AB = 49,    // SVF AB Input Mix (Amount)
  SVF_CMIX = 50,  // SVF Comb Input Mix (Amount)
  SVF_F1_CUT
  = 51,  // SVF Upper Filter Frequency (Cutoff, Cutoff KT, Cutoff Env C Amount, Env C, Spread, Master Tune, Reference, Key Pitch)
  SVF_F2_CUT
  = 52,  // SVF Lower Filter Frequency (Cutoff, Cutoff KT, Cutoff Env C Amount, Env C, Spread, Master Tune, Reference, Key Pitch)
  SVF_F1_FM
  = 53,  // SVF Upper Filter FM Amount (Cutoff, Cutoff KT, Cutoff Env C Amount, Env C, Spread, FM, Master Tune, Reference, Key Pitch)
  SVF_F2_FM
  = 54,  // SVF Lower Filter FM Amount (Cutoff, Cutoff KT, Cutoff Env C Amount, Env C, Spread, FM, Master Tune, Reference, Key Pitch)
  SVF_RES = 55,    // SVF Resonance (old) (Resonance, Res KT, Res Env C Amount, Env C, Master Tune, Key Pitch)
  SVF_RES_DAMP = 56,    // SVF Resonance (new) - damp factor (derived from res in param engine)
  SVF_RES_FMAX = 57,    // SVF Resonance (new) - freq maximum (derived from res in param engine)
  SVF_LBH_1 = 58,  // SVF Upper LBH Mix (Amount)
  SVF_LBH_2 = 59,  // SVF Lower LBH Mix (Amount)
  SVF_PAR_1 = 60,  // SVF Parallel Amount 1 (Amount)
  SVF_PAR_2 = 61,  // SVF Parallel Amount 2 (Amount)
  SVF_PAR_3 = 62,  // SVF Parallel Amount 3 (Amount)
  SVF_PAR_4 = 63,  // SVF Parallel Amount 4 (Amount)
  SVF_FMAB = 64,   // SVF FM AB Mix (Amount)

  /* Feedback Mixer Signals           -- polyphonic */

  FBM_CMB = 65,  // FB Mix Comb Mix (Amount)
  FBM_SVF = 66,  // FB Mix SVF Mix (Amount)
  FBM_FX = 67,   // FB Mix Effects Mix (Amount)
  FBM_REV = 68,  // FB Mix Reverb Mix (Amount)
  FBM_DRV = 69,  // FB Mix Drive (Amount)
  FBM_FLD = 70,  // FB Mix Fold (Amount)
  FBM_ASM = 71,  // FB Mix Asymetry (Amount)
  FBM_LVL = 72,  // FB Mix Level (Level, Level KT, Master Tune, Key Pitch)
  FBM_HPF = 73,  // FB Mix Highpass Frequency (Master Tune, Key Pitch)

  /* Output Mixer Signals             -- polyphonic */

  OUT_A_L = 74,    // Out Mix Branch A Left Level (Level, Pan, Key Pan)
  OUT_A_R = 75,    // Out Mix Branch A Right Level (Level, Pan, Key Pan)
  OUT_B_L = 76,    // Out Mix Branch B Left Level (Level, Pan, Key Pan)
  OUT_B_R = 77,    // Out Mix Branch B Right Level (Level, Pan, Key Pan)
  OUT_CMB_L = 78,  // Out Mix Comb Left Level (Level, Pan, Key Pan)
  OUT_CMB_R = 79,  // Out Mix Comb Right Level (Level, Pan, Key Pan)
  OUT_SVF_L = 80,  // Out Mix SVF Left Level (Level, Pan, Key Pan)
  OUT_SVF_R = 81,  // Out Mix SVF Right Level (Level, Pan, Key Pan)
  OUT_DRV = 82,    // Out Mix Drive (Amount)
  OUT_FLD = 83,    // Out Mix Fold (Amount)
  OUT_ASM = 84,    // Out Mix Asymetry (Amount)
  OUT_LVL = 85,    // Out Mix Level (Level)

  /* Cabinet Signals                  -- monophonic */

  CAB_DRV = 86,     // Cabinet Drive (Amount)
  CAB_FLD = 87,     // Cabinet Fold (Amount)
  CAB_ASM = 88,     // Cabinet Asymetry (Amount)
  CAB_PRESAT = 89,  // Cabinet Saturation Level (Tilt - pre Shaper factor)
  CAB_SAT = 90,     // Cabinet Saturation Level (Tilt - post Shaper factor)
  CAB_TILT = 91,    // Cabinet Tilt (Tilt)
  CAB_LPF = 92,     // Cabinet Hi Cut (Cutoff)
  CAB_HPF = 93,     // Cabinet Lo Cut (Cutoff)
  CAB_DRY = 94,     // Cabinet Dry Amount (Mix)
  CAB_WET = 95,     // Cabinet Wet Amount (Mix)

  /* Gap Filter Signals               -- monophonic */

  GAP_LFL = 96,     // Gap Filter Left Lowpass Frequency (Center, Stereo, Gap)
  GAP_HFL = 97,     // Gap Filter Left Highpass Frequency (Center, Stereo, Gap)
  GAP_LFR = 98,     // Gap Filter Right Lowpass Frequency (Center, Stereo, Gap)
  GAP_HFR = 99,     // Gap Filter Right Highpass Frequency (Center, Stereo, Gap)
  GAP_RES = 100,     // Gap Filter Resonance (Amount)
  GAP_HPLP = 101,    // Gap Filter Highpass to Lowpass Amount (Balance, Mix)
  GAP_INLP = 102,   // Gap Filter Input to Lowpass Amount (Balance, Mix)
  GAP_HPOUT = 103,  // Gap Filter Highpass to Output Amount (Balance, Mix)
  GAP_LPOUT = 104,  // Gap Filter Lowpass to Output Amount (Balance, Mix)
  GAP_INOUT = 105,  // Gap Filter Input to Output Amount (Balance, Mix)

  /* Flanger Signals                  -- monophonic */

  FLA_TMOD = 106,    // Flanger Time Modulation (Amount)
  FLA_LFO_L = 107,   // Flanger Left Modulation Signal (Phase, Rate, Envelope, Flanger LFO/Envelope)
  FLA_LFO_R = 108,   // Flanger Right Modulation Signal (Phase, Rate, Envelope, Flanger LFO/Envelope)
  FLA_TL = 109,      // Flanger Left Time (Time, Stereo)
  FLA_TR = 110,      // Flanger Right Time (Time, Stereo)
  FLA_FB_LOC = 111,  // Flanger Local Feedback Amount (Feedback, Cross FB)
  FLA_FB_CR = 112,   // Flanger Cross Feedback Amount (Feedback, Cross FB)
  FLA_LPF = 113,     // Flanger Lowpass Frequency (Hi Cut)
  FLA_DRY = 114,     // Flanger Dry Amount (Mix)
  FLA_WET = 115,     // Flanger Wet Amount (Mix)
  FLA_APF_L = 116,   // Flanger Left Allpass Frequency (AP Mod, AP Tune, Flanger LFO/Envelope)
  FLA_APF_R = 117,   // Flanger Right Allpass Frequency (AP Mod, AP Tune, Flanger LFO/Envelope)

  /* Echo Signals                     -- monophonic */

  DLY_TL = 118,      // Echo Left Time (Time, Stereo)
  DLY_TR = 119,      // Echo Right Time (Time, Stereo)
  DLY_FB_LOC = 120,  // Echo Local Feedback (Feedback, Cross FB)
  DLY_FB_CR = 121,   // Echo Cross Feedback (Feedback, Cross FB)
  DLY_LPF = 122,     // Echo Lowpass Frequency (Hi Cut)
  DLY_DRY = 123,     // Echo Dry Amount (Mix)
  DLY_WET = 124,     // Echo Wet Amount (Mix)
  DLY_SND = 125,     // Echo Send Amount

  /* Reverb Signals                   -- monophonic */

  REV_SIZE = 126,  // Reverb Size (Size)
  REV_FEED = 127,  // Reverb Feedback Amount (Size)
  REV_BAL = 128,   // Reverb Balance (Size)
  REV_PRE = 129,   // Reverb Pre Delay (Pre Delay)
  REV_HPF = 130,   // Reverb Highpass Frequency (Color)
  REV_LPF = 131,   // Reverb Lowpass Frequency (Color)
  REV_CHO = 132,   // Reverb Chorus (Chorus)
  REV_DRY = 133,   // Reverb Dry Amount (Mix)
  REV_WET = 134,   // Reverb Wet Amount (Mix)
  REV_SND = 135,   // Reverb Send Amount

  /* Master Signals                   -- monophonic */

  MST_VOL = 136,  // Master Volume (Volume)

  /* Unison Signals                   -- polyphonic */

  UN_PHS = 137  // Unison Phase Offset
};
