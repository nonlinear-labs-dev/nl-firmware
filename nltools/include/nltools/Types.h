#pragma once
#include "nltools/enums/EnumTools.h"

ENUM(SoundType, int8_t, Single, Split, Layer, Invalid);
ENUM(MacroControls, int8_t, NONE = 0, MC1 = 1, MC2 = 2, MC3 = 3, MC4 = 4, NUM_CHOICES = 5);
ENUM(VoiceGroup, int8_t, I, II, Global, NumGroups);
ENUM(RibbonTouchBehaviour, int8_t, ABSOLUTE, RELATIVE, NUM_TOUCH_BEHAVIOURS);
ENUM(RibbonReturnMode, int8_t, STAY = 0, RETURN = 1, NUM_RETURN_MODES = 2);
ENUM(PedalModes, int8_t, STAY = 0, RETURN_TO_ZERO = 1, RETURN_TO_CENTER = 2, NUM_PEDAL_MODES = 3);
ENUM(ReturnMode, int8_t, None, Center, Zero);
