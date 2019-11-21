#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/VoiceGroupSelection.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button(getTextFor(getCurrentSelection()), pos)
{
  Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(sigc::mem_fun(this, &SwitchVoiceGroupButton::update));
}

Glib::ustring SwitchVoiceGroupButton::getTextFor(VoiceGroup vg)
{
  if(vg == VoiceGroup::I)
    return "Select II";
  else
    return "Select I";
}

VoiceGroup SwitchVoiceGroupButton::getCurrentSelection()
{
  return Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
}

void SwitchVoiceGroupButton::update()
{
  setText({getTextFor(getCurrentSelection()), 0});
}