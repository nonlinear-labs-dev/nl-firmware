#pragma once

#include "Button.h"
#include <nltools/Types.h>
#include <libundo/undo/TransactionCreationScope.h>

class Parameter;

class SwitchVoiceGroupButton : public Button
{
 public:
  explicit SwitchVoiceGroupButton(Buttons pos);

  static std::unique_ptr<UNDO::TransactionCreationScope> createToggleVoiceGroupWithParameterHighlightScope();

 private:
  static Glib::ustring getTextFor(VoiceGroup vg);

  void rebuild();

  void onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection);
  void onVoiceGroupChanged(VoiceGroup newVoiceGroup);
};
