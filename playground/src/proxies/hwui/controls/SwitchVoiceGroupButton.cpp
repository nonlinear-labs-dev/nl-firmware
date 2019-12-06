#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/VoiceGroupSelection.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/mono-mode-parameters/MonoParameter.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button(getTextFor(Application::get().getHWUI()->getCurrentVoiceGroup()), pos)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::onParameterSelectionChanged));
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

void SwitchVoiceGroupButton::onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection)
{
  if(!newSelection)
    return;

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto newSelectionVoiceGroup = newSelection->getID().getVoiceGroup();

  if(eb->getType() == SoundType::Single || newSelectionVoiceGroup == VoiceGroup::Global)
    setText({ "", 0 });
  else if(eb->getType() == SoundType::Layer && dynamic_cast<MonoParameter*>(newSelection))
    setText({ "", 0 });
  else
    setText({ getTextFor(newSelectionVoiceGroup), 0 });
}
