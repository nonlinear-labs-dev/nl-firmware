#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button(getTextFor(Application::get().getHWUI()->getCurrentVoiceGroup()), pos)
{
  Application::get().getHWUI()->onCurrentVoiceGroupChanged(sigc::mem_fun(this, &SwitchVoiceGroupButton::update));
}

Glib::ustring SwitchVoiceGroupButton::getTextFor(VoiceGroup vg)
{
  if(vg == VoiceGroup::Global)
    return "";
  if(vg == VoiceGroup::I)
    return "Select II";
  else
    return "Select I";
}

void SwitchVoiceGroupButton::update(VoiceGroup newVoiceGroup)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(eb->getType() == SoundType::Single)
    setText({ "", 0 });
  else
    setText({ getTextFor(newVoiceGroup), 0 });
}