#include "SwitchVoiceGroupButton.h"
#include "groups/MasterGroup.h"
#include "groups/ScaleGroup.h"
#include "groups/MacroControlsGroup.h"
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

SwitchVoiceGroupButton::SwitchVoiceGroupButton(Buttons pos)
    : Button("", pos)
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::onParameterSelectionChanged), currentVG);

  Application::get().getVGManager()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::onVoiceGroupChanged));

  Application::get().getPresetManager()->getEditBuffer()->onPresetLoaded(
      sigc::mem_fun(this, &SwitchVoiceGroupButton::rebuild));

  rebuild();
}

namespace
{
  bool isAnyScaleParameterChanged()
  {
    auto& eb = *Application::get().getPresetManager()->getEditBuffer();
    auto group = eb.getParameterGroupByID({ "Scale", VoiceGroup::Global });
    return group->isAnyParameterChanged();
  }
}

void SwitchVoiceGroupButton::rebuild()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto selected = eb->getSelected(vg);

  if(allowToggling(selected, eb))
    setText(StringAndSuffix { "I / II", 0 });
  else if(MasterGroup::isMasterParameter(selected) && selected->getID().getNumber() != C15::PID::Master_FX_Mix)
  {
    if(isAnyScaleParameterChanged())
      setText(StringAndSuffix { "Scale..*", 0 });
    else
      setText(StringAndSuffix { "Scale..", 0 });
  }
  else if(ScaleGroup::isScaleParameter(selected))
  {
    setText(StringAndSuffix { "back..", 0 });
  }
  else
  {
    setText(StringAndSuffix { "", 0 });
  }
}

void SwitchVoiceGroupButton::onParameterSelectionChanged(Parameter* oldSelected, Parameter* newSelection)
{
  rebuild();
}

void SwitchVoiceGroupButton::onVoiceGroupChanged(VoiceGroup newVoiceGroup)
{
  rebuild();
}

bool SwitchVoiceGroupButton::allowToggling(const Parameter* selected, const EditBuffer* editBuffer)
{
  if(selected == nullptr)
    return false;

  if(selected->getVoiceGroup() == VoiceGroup::Global)
  {
    return MacroControlsGroup::isMacroControl(selected->getID().getNumber());
  }

  if(editBuffer->getType() == SoundType::Single)
  {
    const auto type = selected->getType();
    if(type == C15::Descriptors::ParameterType::Monophonic_Modulateable
       || type == C15::Descriptors::ParameterType::Monophonic_Unmodulateable)
    {
      return true;
    }
    return false;
  }

  auto layerAndGroupAllowToggling
      = ((editBuffer->getType() == SoundType::Layer)
         && (!MonoGroup::isMonoParameter(selected) && !UnisonGroup::isUnisonParameter(selected)))
      || (editBuffer->getType() != SoundType::Layer);

  if(!selected->isDisabled())
    return layerAndGroupAllowToggling;
  else
    return false;
}
