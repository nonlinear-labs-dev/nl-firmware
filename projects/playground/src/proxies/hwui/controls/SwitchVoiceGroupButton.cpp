#include "SwitchVoiceGroupButton.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/SplitPointParameter.h>
#include <groups/MonoGroup.h>
#include <proxies/hwui/HWUI.h>
#include <groups/UnisonGroup.h>
#include <nltools/Types.h>
#include <libundo/undo/Scope.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <parameter_declarations.h>
#include <proxies/hwui/HWUI.h>

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button("", pos)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onSelectionChanged(sigc::mem_fun(this, &SwitchVoiceGroupButton::onParameterSelectionChanged));
  eb->onCurrentVoiceGroupChanged(sigc::mem_fun(this, &SwitchVoiceGroupButton::onVoiceGroupChanged));
  eb->onPresetLoaded(sigc::mem_fun(this, &SwitchVoiceGroupButton::rebuild));
}

void SwitchVoiceGroupButton::rebuild()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto selected = eb->getSelected();

  if(allowToggling(selected, eb))
    setText({ "I / II", 0 });
  else
    setText({ "", 0 });
}

void SwitchVoiceGroupButton::onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection)
{
  rebuild();
}

void SwitchVoiceGroupButton::onVoiceGroupChanged(VoiceGroup newVoiceGroup)
{
  rebuild();
}

bool SwitchVoiceGroupButton::toggleVoiceGroup()
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  auto selected = eb->getSelected();

  if(dynamic_cast<const SplitPointParameter*>(selected))
  {
    eb->toggleCurrentVoiceGroup();
    return true;
  }

  auto layoutSwitchLock = GenericScopeGuard([]() { Application::get().getHWUI()->freezeFocusAndMode(); },
                                            []() { Application::get().getHWUI()->thawFocusAndMode(); });

  if(allowToggling(selected, eb))
  {
    auto otherVG = selected->getVoiceGroup() == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
    if(auto other = eb->findParameterByID({ selected->getID().getNumber(), otherVG }))
    {
      auto scope = pm->getUndoScope().startContinuousTransaction(&other, std::chrono::hours(1), "Select '%0'",
                                                                 other->getGroupAndParameterNameWithVoiceGroup());
      eb->toggleCurrentVoiceGroupAndUpdateParameterSelection(scope->getTransaction());
      return true;
    }
  }

  return false;
}

bool SwitchVoiceGroupButton::allowToggling(const Parameter* selected, const EditBuffer* editBuffer)
{
  if(selected->getVoiceGroup() == VoiceGroup::Global)
    return selected->getID().getNumber() == C15::PID::Split_Split_Point;

  if(editBuffer->getType() == SoundType::Single)
    return false;

  auto layerAndGroupAllowToggling
      = ((editBuffer->getType() == SoundType::Layer)
         && (!MonoGroup::isMonoParameter(selected) && !UnisonGroup::isUnisonParameter(selected)))
      || (editBuffer->getType() != SoundType::Layer);

  if(ParameterLayout2::isParameterAvailableInSoundType(selected, editBuffer))
    return layerAndGroupAllowToggling;
  else
    return false;
}
