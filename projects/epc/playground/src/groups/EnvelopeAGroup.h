#pragma once

#include "ParameterGroup.h"

class EnvelopeAGroup : public ParameterGroup
{
 public:
  EnvelopeAGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  virtual ~EnvelopeAGroup();

  void init();
};
