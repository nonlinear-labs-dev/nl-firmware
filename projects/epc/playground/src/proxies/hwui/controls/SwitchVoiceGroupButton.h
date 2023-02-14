#pragma once

#include "Button.h"
#include <nltools/Types.h>
#include <libundo/undo/TransactionCreationScope.h>

class Parameter;
class EditBuffer;

class SwitchVoiceGroupButton : public Button
{
 public:
  explicit SwitchVoiceGroupButton(Buttons pos);

 private:
  void rebuild();

  void onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection);
  void onVoiceGroupChanged(VoiceGroup newVoiceGroup);
};
