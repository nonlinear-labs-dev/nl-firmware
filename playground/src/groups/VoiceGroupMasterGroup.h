#pragma once

#include "ParameterGroup.h"

class VoiceGroupMasterGroup : public ParameterGroup
{
public:
  VoiceGroupMasterGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  void init() override;
};
