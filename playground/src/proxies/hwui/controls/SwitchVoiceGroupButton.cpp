#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <groups/MonoGroup.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <proxies/hwui/HWUI.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button(getTextFor(Application::get().getHWUI()->getCurrentVoiceGroup()), pos)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::onParameterSelectionChanged));

  Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::onVoiceGroupChanged));
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

void SwitchVoiceGroupButton::rebuild()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto ebType = eb->getType();
  auto selected = eb->getSelected();
  auto selectedVoiceGroup = Application::get().getHWUI()->getCurrentVoiceGroup();

  switch(ebType)
  {
    case SoundType::Layer:
    {
      if(MonoGroup::isMonoParameter(selected) && !dynamic_cast<const MonoGlideTimeParameter*>(selected))
      {
        setText({ "", 0 });
        return;
      }
    }
    break;
    case SoundType::Split:
    {
      if(dynamic_cast<SplitPointParameter*>(selected) != nullptr)
      {
        setText({ getTextFor(selectedVoiceGroup), 0 });
        return;
      }
    }
    break;
    case SoundType::Single:
    {
      setText({ "", 0 });
      return;
    }
    break;
  }

  setText({ getTextFor(selected->getVoiceGroup()), 0 });
}

void SwitchVoiceGroupButton::onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection)
{
  rebuild();
}

void SwitchVoiceGroupButton::onVoiceGroupChanged(VoiceGroup newVoiceGroup)
{
  rebuild();
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
