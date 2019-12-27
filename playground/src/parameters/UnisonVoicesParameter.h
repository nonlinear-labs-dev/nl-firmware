#pragma once
#include "parameters/unison-parameters/UnmodulateableUnisonParameter.h"

class UnisonVoicesParameter : public UnmodulateableUnisonParameter
{
 public:
  explicit UnisonVoicesParameter(ParameterGroup* group, VoiceGroup vg);
  void updateScaling(SoundType type);
};
