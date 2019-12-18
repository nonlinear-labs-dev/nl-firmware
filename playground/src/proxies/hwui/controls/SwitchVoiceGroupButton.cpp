#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <groups/MonoGroup.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>

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
  else if(eb->getType() == SoundType::Layer && MonoGroup::isMonoParameter(newSelection)
          && !dynamic_cast<const MonoGlideTimeParameter*>(newSelection))
    setText({ "", 0 });
  else
    setText({ getTextFor(newSelectionVoiceGroup), 0 });
}

std::unique_ptr<UNDO::TransactionCreationScope>
    SwitchVoiceGroupButton::createToggleVoiceGroupWithParameterHighlightScope()
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  auto selected = eb->getSelected();

  auto hasCounterPart = selected->getVoiceGroup() != VoiceGroup::Global;

  if(hasCounterPart)
  {
    auto otherVG = selected->getVoiceGroup() == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
    auto other = eb->findParameterByID({ selected->getID().getNumber(), otherVG });
    return pm->getUndoScope().startContinuousTransaction(&other, std::chrono::hours(1), "Select '%0'",
                                                         other->getGroupAndParameterNameWithVoiceGroup());
  }
  else
  {
    return UNDO::Scope::startTrashTransaction();
  }
}
