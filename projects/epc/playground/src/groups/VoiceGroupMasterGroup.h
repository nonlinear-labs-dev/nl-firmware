#pragma once

#include "ParameterGroup.h"

class VoiceGroupMasterGroup : public ParameterGroup
{
 public:
  VoiceGroupMasterGroup(ParameterGroupSet *parent, VoiceGroup voicegroup);
  static bool isVoiceGroupMasterParameter(int id);
  void init() override;
};
