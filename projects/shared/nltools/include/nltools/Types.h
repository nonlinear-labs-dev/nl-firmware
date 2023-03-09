#pragma once

#include "nltools/enums/EnumTools.h"
#include <nltools/GlibFwd.h>
#include <limits>
#include <type_traits>

ENUM(SoundType, int8_t, Single, Split, Layer, Invalid);
ENUM(MacroControls, int8_t, NONE = 0, MC1 = 1, MC2 = 2, MC3 = 3, MC4 = 4, MC5 = 5, MC6 = 6, NUM_CHOICES = 7);
ENUM(VoiceGroup, int8_t, I = 0, II = 1, Global = 2, NumGroups = 3, Invalid = 4);
ENUM(RibbonTouchBehaviour, int8_t, ABSOLUTE, RELATIVE, NUM_TOUCH_BEHAVIOURS);
ENUM(RibbonReturnMode, int8_t, STAY = 0, RETURN = 1, NUM_RETURN_MODES = 2);
ENUM(PedalModes, int8_t, STAY = 0, RETURN_TO_ZERO = 1, RETURN_TO_CENTER = 2, NUM_PEDAL_MODES = 3);
ENUM(ReturnMode, int8_t, None, Center, Zero);
ENUM(PresetStoreModeSettings, int8_t, PRESET_STORE_MODE_APPEND, PRESET_STORE_MODE_OVERWRITE, PRESET_STORE_MODE_INSERT);
ENUM(BaseUnitUIModes, int8_t, Play = 0, ParameterEdit = 1, Banks = 2, Presets = 3);
ENUM(BaseUnitUIDetails, int8_t, RibbonSelect, RibbonMode);
ENUM(ParameterEditModeRibbonBehaviours, int8_t, PARAMETER_EDIT_MODE_RIBBON_BEHAVIOUR_RELATIVE = 0,
     PARAMETER_EDIT_MODE_RIBBON_BEHAVIOUR_ABSOLUTE = 1);
ENUM(DebugLevels, int8_t, DEBUG_LEVEL_SILENT, DEBUG_LEVEL_ERROR, DEBUG_LEVEL_WARNING, DEBUG_LEVEL_INFO,
     DEBUG_LEVEL_DEBUG, DEBUG_LEVEL_GASSY);
ENUM(BooleanSettings, int8_t, BOOLEAN_SETTING_TRUE = 0, BOOLEAN_SETTING_FALSE = 1);

ENUM(PedalTypes, int, PotTipActive = 1, PotRingActive = 2, PotTipActiveReverse = 3, PotRingActiveReverse = 4,
     Resistor = 5, ResistorReversed = 6, SwitchClosing = 7, SwitchOpening = 8, CV0to5V = 9, CV0To5VAutoRange = 10,
     OFF = 11, BossEV30 = 12, BossFV500L = 13, DoepferFP5 = 14, FractalEV2 = 15, KorgDS1H = 16, KorgEXP2 = 17,
     LeadFootLFX1 = 18, MAudioEXP = 19, MoogEP3 = 20, RolandDP10 = 21, RolandEV5 = 22, YamahaFC3A = 23, YamahaFC7 = 24);

ENUM(BenderCurves, int8_t, Soft = 0, Normal = 1, Hard = 2);
ENUM(WifiSettings, int8_t, Disabled, Enabled);
ENUM(SelectedRibbons, int8_t, Ribbon1_2, Ribbon3_4);

ENUM(MidiReceiveChannel, int8_t, None, Omni, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9, CH_10, CH_11, CH_12,
     CH_13, CH_14, CH_15, CH_16)

ENUM(MidiReceiveChannelSplit, int8_t, None, Omni, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9, CH_10, CH_11,
     CH_12, CH_13, CH_14, CH_15, CH_16, Common)

ENUM(MidiSendChannel, int8_t, None, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9, CH_10, CH_11, CH_12, CH_13,
     CH_14, CH_15, CH_16)

ENUM(MidiSendChannelSplit, int8_t, None, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9, CH_10, CH_11, CH_12,
     CH_13, CH_14, CH_15, CH_16, Common)

ENUM(RibbonCC, int8_t, None, CC01, CC02, CC03, CC04, CC05, CC06, CC07, CC08, CC09, CC10, CC11, CC12, CC13, CC14, CC15,
     CC16, CC17, CC18, CC19, CC20, CC21, CC22, CC23, CC24, CC25, CC26, CC27, CC28, CC29, CC30, CC31);

ENUM(PedalCC, int8_t, None, CC01, CC02, CC03, CC04, CC05, CC06, CC07, CC08, CC09, CC10, CC11, CC12, CC13, CC14, CC15,
     CC16, CC17, CC18, CC19, CC20, CC21, CC22, CC23, CC24, CC25, CC26, CC27, CC28, CC29, CC30, CC31, CC64, CC65, CC66,
     CC67, CC68, CC69, CC64_Continuous, CC65_Continuous, CC66_Continuous, CC67_Continuous, CC68_Continuous,
     CC69_Continuous);

ENUM(BenderCC, int8_t, None, Pitchbend, CC01, CC02, CC03, CC04, CC05, CC06, CC07, CC08, CC09, CC10, CC11, CC12, CC13,
     CC14, CC15, CC16, CC17, CC18, CC19, CC20, CC21, CC22, CC23, CC24, CC25, CC26, CC27, CC28, CC29, CC30, CC31);

ENUM(AftertouchCC, int8_t, None, ChannelPressure, CC01, CC02, CC03, CC04, CC05, CC06, CC07, CC08, CC09, CC10, CC11,
     CC12, CC13, CC14, CC15, CC16, CC17, CC18, CC19, CC20, CC21, CC22, CC23, CC24, CC25, CC26, CC27, CC28, CC29, CC30,
     CC31, PitchbendUp, PitchbendDown);

ENUM(AftertouchCurves, int8_t, AFTERTOUCH_CURVE_SOFT = 0, AFTERTOUCH_CURVE_NORMAL = 1, AFTERTOUCH_CURVE_HARD = 2)

ENUM(VelocityCurves, int8_t, VELOCITY_CURVE_VERY_SOFT = 0, VELOCITY_CURVE_SOFT = 1, VELOCITY_CURVE_NORMAL = 2,
     VELOCITY_CURVE_HARD = 3, VELOCITY_CURVE_VERY_HARD = 4)

ENUM(HWChangeSource, int8_t, TCD = 0, MIDI = 1, UI = 2)

ENUM(MidiHWChangeSpecialCases, uint16_t, ChannelPitchbend, Aftertouch, PitchbendUp, PitchbendDown, CC)

ENUM(EPC_MODEL, uint16_t, UNKNOWN, i3_5, i3_7, i3_10, i3_11, i5_7)

template <typename T> inline typename std::enable_if<std::is_floating_point<T>::value, bool>::type differs(T a, T b)
{
  return std::abs(a - b) > std::numeric_limits<T>::epsilon();
}

#ifdef _DEVELOPMENT_PC
constexpr auto isDevelopmentPC = true;
#else
constexpr auto isDevelopmentPC = false;
#endif
