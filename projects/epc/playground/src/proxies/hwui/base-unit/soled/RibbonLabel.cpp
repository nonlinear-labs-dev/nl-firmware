#include "RibbonLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/PhysicalControlParameter.h"
#include "proxies/hwui/HWUI.h"
#include <groups/ParameterGroup.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <parameter_declarations.h>
#include <device-settings/GlobalLocalEnableSetting.h>

RibbonLabel::RibbonLabel(const ParameterId &paramID, const Rect &rect)
    : super(rect)
    , m_parameterID(paramID)
{
  auto settings = Application::get().getSettings();
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->getParameterGroupByID({ "CS", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  eb->getParameterGroupByID({ "MCs", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  eb->getParameterGroupByID({ "MCM", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  settings->onSettingsChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  m_parameter = eb->findAndCastParameterByID<PhysicalControlParameter>(m_parameterID);
}

RibbonLabel::~RibbonLabel() = default;

StringAndSuffix RibbonLabel::getText() const
{
  using rIDX = RoutingSettings::tRoutingIndex;
  using aIDX = RoutingSettings::tAspectIndex;

  static auto settings = Application::get().getSettings();
  static const auto routingsSetting = settings->getSetting<RoutingSettings>();

  const auto isRibbon1 = m_parameterID == ParameterId { C15::PID::Ribbon_1, VoiceGroup::Global };
  const auto isRibbon2 = m_parameterID == ParameterId { C15::PID::Ribbon_2, VoiceGroup::Global };

  const auto isLocalEnabled = settings->getSetting<GlobalLocalEnableSetting>()->get();

  const auto isRibbon1Enabled = isRibbon1 && routingsSetting->getState(rIDX::Ribbon1, aIDX::LOCAL) && isLocalEnabled;
  const auto isRibbon2Enabled = isRibbon2 && routingsSetting->getState(rIDX::Ribbon2, aIDX::LOCAL) && isLocalEnabled;
  const auto isRibbonEnabled = isRibbon1Enabled || isRibbon2Enabled;

  if(isRibbonEnabled && m_parameter)
  {
    return StringAndSuffix { crop(m_parameter->generateName()) };
  }
  else
  {
    if(isRibbon1)
      return cropMidiCC(settings->getSetting<RibbonCCMapping<1>>()->getDisplayString());
    else if(isRibbon2)
      return cropMidiCC(settings->getSetting<RibbonCCMapping<2>>()->getDisplayString());
  }

  return "";
}

Glib::ustring RibbonLabel::cropMidiCC(const Glib::ustring &text)
{
  //CC XX (LSB: CC YY)
  //CC XX

  if(text.find('(') != Glib::ustring::npos)
  {
    auto start = text.find(':') + 2;
    auto end = text.find(')');
    auto lsbText = text.substr(start, end - start);
    auto msbEnd = text.find('(') - 1;
    auto msbText = text.substr(0, msbEnd);
    return msbText + " (" + lsbText + ")";
  }
  return text;
}

Glib::ustring RibbonLabel::crop(const Glib::ustring &text) const
{
  auto min = 0ul;
  auto max = text.length();

  auto w = getFont()->getStringWidth(text);

  if(w <= getPosition().getWidth())
    return text;

  return binarySearchLength(text, min, max);
}

Glib::ustring RibbonLabel::binarySearchLength(const Glib::ustring &text, unsigned long min, unsigned long max) const
{
  if(max == 0)
    return "";

  if((min + 1) == max)
    return text.substr(0, min) + "..";

  auto halfIdx = min + (max - min) / 2;
  auto half = text.substr(0, halfIdx);
  auto toMeasure = half + "..";

  auto w = getFont()->getStringWidth(toMeasure);

  if(w <= getPosition().getWidth())
    min = halfIdx;
  else
    max = halfIdx;

  return binarySearchLength(text, min, max);
}
