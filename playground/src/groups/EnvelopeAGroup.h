#pragma once

#include "ParameterGroup.h"

class EnvelopeAGroup : public ParameterGroup
{
 public:
  EnvelopeAGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~EnvelopeAGroup();

  void init();
};
