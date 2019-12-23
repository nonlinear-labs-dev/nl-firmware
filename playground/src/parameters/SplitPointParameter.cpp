#include "SplitPointParameter.h"

#include <parameters/scale-converters/KeyWithOctaveScaleConverter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/DualVoiceGroupMasterAndSplitPointLayout.h>
#include "groups/ParameterGroup.h"
#include "proxies/hwui/HWUI.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <parameters/scale-converters/dimension/NoteWithOctaveDimension.h>

SplitPointParameter::SplitPointParameter(ParameterGroup *group, const ParameterId &id)
    : ModulateableParameter(group, id, ScaleConverter::get<KeyWithOctaveScaleConverter>(), 0.5, 60, 60)
{
}

Glib::ustring SplitPointParameter::getGroupAndParameterName() const
{
  return "Split Point";
}

DFBLayout *SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new DualVoiceGroupMasterAndSplitPointLayout();
  }
}

std::string SplitPointParameter::getDisplayValue(VoiceGroup vg) const
{
  if(vg == VoiceGroup::I)
    return NoteWithOctaveDimension::stringizeNote(static_cast<int>(std::ceil(getValue().getRawValue() * 59)));
  else if(vg == VoiceGroup::II)
    return NoteWithOctaveDimension::stringizeNote(static_cast<int>(std::ceil(getValue().getRawValue() * 59)) + 1);

  return "";
}

Glib::ustring SplitPointParameter::getDisplayString() const
{
  return getDisplayValue(Application::get().getHWUI()->getCurrentVoiceGroup());
}

Glib::ustring SplitPointParameter::getLongName() const
{
  return "Split Point";
}

Glib::ustring SplitPointParameter::getShortName() const
{
  return "Split P.";
}