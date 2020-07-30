#include "SplitPointParameter.h"
#include <parameters/scale-converters/LinearBipolar59StScaleConverter.h>
#include <parameters/scale-converters/SplitPointScaleConverter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableDualVoiceGroupMasterAndSplitPointLayout.h>
#include "groups/ParameterGroup.h"
#include "proxies/hwui/HWUI.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

SplitPointParameter::SplitPointParameter(ParameterGroup *group, const ParameterId &id)
    : ModulateableParameterWithUnusualModUnit(group, id, ScaleConverter::get<SplitPointScaleConverter>(),
                                              ScaleConverter::get<LinearBipolar59StScaleConverter>(), 0.5, 59, 59)
{
}

Layout *SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new ModulateableDualVoiceGroupMasterAndSplitPointLayout();
  }
}

Glib::ustring SplitPointParameter::getDisplayString(VoiceGroup vg) const
{
  return SplitPointDimension::stringizeSplitPointDisplay(getValue().getRawValue(), vg);
}

Glib::ustring SplitPointParameter::getDisplayString() const
{
  return getDisplayString(Application::get().getPresetManager()->getEditBuffer()->getCurrentHWUIVoiceGroup());
}

Glib::ustring SplitPointParameter::stringizeModulationAmount(tControlPositionValue amount) const
{
  return std::to_string(static_cast<int>(60 * amount)) + " st";
}

Glib::ustring SplitPointParameter::modulationValueToDisplayString(tControlPositionValue v) const
{
  auto vg = Application::get().getPresetManager()->getEditBuffer()->getCurrentHWUIVoiceGroup();
  return SplitPointDimension::stringizeSplitPointDisplay(v, vg);
}

Glib::ustring SplitPointParameter::getDisplayString(VoiceGroup vg, tControlPositionValue display) const
{
  return SplitPointDimension::stringizeSplitPointDisplay(display, vg);
}

Glib::ustring SplitPointParameter::getDisplayString(tControlPositionValue cp) const
{
  return getDisplayString(Application::get().getPresetManager()->getEditBuffer()->getCurrentHWUIVoiceGroup(), cp);
}
