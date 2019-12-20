#pragma once
#include "Parameter.h"

class UnisonVoicesParameter : public Parameter
{
 public:
  explicit UnisonVoicesParameter(ParameterGroup* group, VoiceGroup vg);
  void updateScaling(SoundType type);
};
