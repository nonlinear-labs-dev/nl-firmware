#pragma once

#include "ParameterGroup.h"
#include "parameters/ModulateableParameter.h"

class MacroControlsGroup : public ParameterGroup
{
 public:
  MacroControlsGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~MacroControlsGroup();

  void init();

  static int modSrcToParamNumber(MacroControls src);
  static MacroControls paramIDToModSrc(int pid);
};
