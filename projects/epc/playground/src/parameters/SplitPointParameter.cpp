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
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <parameter_declarations.h>

SplitPointParameter::SplitPointParameter(ParameterGroup* group, const ParameterId& id)
    : ModulateableParameterWithUnusualModUnit(group, id, ScaleConverter::get<SplitPointScaleConverter>(),
                                              ScaleConverter::get<LinearBipolar59StScaleConverter>(), 0.5, 59, 59)
{
  m_syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>().get();
}

Layout* SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new SplitPointParameterEditLayout();
    case UIMode::Select:
    default:
      return new SplitPointParameterLayout();
  }
}

void SplitPointParameter::setCpValue(UNDO::Transaction* transaction, Initiator initiator, tControlPositionValue value,
                                     bool dosendToPlaycontroller)
{
  Parameter::setCpValue(transaction, initiator, value, dosendToPlaycontroller);

  if(m_syncSetting->getState() && initiator != Initiator::INDIRECT_SPLIT_SYNC)
  {
    getSibling()->setCpValue(transaction, Initiator::INDIRECT_SPLIT_SYNC, value, dosendToPlaycontroller);
  }
}

Glib::ustring SplitPointParameter::getDisplayString() const
{
  return SplitPointDimension::stringizeSplitPointDisplay(getValue().getRawValue(), getVoiceGroup());
}

Glib::ustring SplitPointParameter::stringizeModulationAmount(tControlPositionValue amount) const
{
  return std::to_string(static_cast<int>(60 * amount)) + " st";
}

Glib::ustring SplitPointParameter::modulationValueToDisplayString(tControlPositionValue v) const
{
  return SplitPointDimension::stringizeSplitPointDisplay(v, getVoiceGroup());
}

Glib::ustring SplitPointParameter::getDisplayString(tControlPositionValue cp) const
{
  return SplitPointDimension::stringizeSplitPointDisplay(cp, getVoiceGroup());
}

SplitPointParameter* SplitPointParameter::getSibling()
{
  return dynamic_cast<SplitPointParameter*>(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
      { C15::PID::Split_Split_Point, getVoiceGroup() == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I }));
}
