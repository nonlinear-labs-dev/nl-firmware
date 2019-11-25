#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/VoiceGroupSelection.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button(getTextFor(Application::get().getHWUI()->getCurrentVoiceGroup()), pos)
{
  Application::get().getHWUI()->onCurrentVoiceGroupChanged(sigc::mem_fun(this, &SwitchVoiceGroupButton::update));
}

Glib::ustring SwitchVoiceGroupButton::getTextFor(VoiceGroup vg)
{
  if(vg == VoiceGroup::I)
    return "Select II";
  else
    return "Select I";
}

void SwitchVoiceGroupButton::update(VoiceGroup newVoiceGroup)
{
  setText({getTextFor(newVoiceGroup), 0});
}