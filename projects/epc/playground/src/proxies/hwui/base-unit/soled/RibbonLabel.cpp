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

RibbonLabel::RibbonLabel(const ParameterId &paramID, const Rect &rect)
    : super(rect)
    , m_parameterID(paramID)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->getParameterGroupByID({ "CS", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  eb->getParameterGroupByID({ "MCs", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  eb->getParameterGroupByID({ "MCM", VoiceGroup::Global })->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
  auto settings = Application::get().getSettings()->onSettingsChanged(sigc::mem_fun(this, &RibbonLabel::setDirty));
}

RibbonLabel::~RibbonLabel() = default;

StringAndSuffix RibbonLabel::getText() const
{
  using rIDX = RoutingSettings::tRoutingIndex;
  using aIDX = RoutingSettings::tAspectIndex;

  static auto settings = Application::get().getSettings();
  static const auto routingsSetting = settings->getSetting<RoutingSettings>();

  const auto isRibbon1Enabled = routingsSetting->getState(rIDX::Ribbon1, aIDX::LOCAL);
  const auto isRibbon2Enabled = routingsSetting->getState(rIDX::Ribbon2, aIDX::LOCAL);

  const auto isRibbon1 = isRibbon1Enabled && m_parameterID == ParameterId { C15::PID::Ribbon_1, VoiceGroup::Global };
  const auto isRibbon2 = isRibbon2Enabled && m_parameterID == ParameterId { C15::PID::Ribbon_2, VoiceGroup::Global };

  const auto isRibbonEnabled = isRibbon1 || isRibbon2;

  if(isRibbonEnabled)
  {
    static auto eb = Application::get().getPresetManager()->getEditBuffer();
    static auto param = dynamic_cast<PhysicalControlParameter *>(eb->findParameterByID(m_parameterID));
    return StringAndSuffix { crop(param->generateName()) };
  }
  else
  {
    if(isRibbon1)
      return crop(settings->getSetting<RibbonCCMapping<1>>()->getDisplayString());
    else if(isRibbon2)
      return crop(settings->getSetting<RibbonCCMapping<2>>()->getDisplayString());
  }
  
  return "";
}

Glib::ustring RibbonLabel::crop(const Glib::ustring &text) const
{
  int min = 0;
  int max = text.length();

  auto w = getFont()->getStringWidth(text);

  if(w <= getPosition().getWidth())
    return text;

  return binarySearchLength(text, min, max);
}

Glib::ustring RibbonLabel::binarySearchLength(const Glib::ustring &text, int min, int max) const
{
  if(max == 0)
    return "";

  if((min + 1) == max)
    return text.substr(0, min) + "..";

  int halfIdx = min + (max - min) / 2;
  Glib::ustring half = text.substr(0, halfIdx);
  Glib::ustring toMeasure = half + "..";

  auto w = getFont()->getStringWidth(toMeasure);

  if(w <= getPosition().getWidth())
    min = halfIdx;
  else
    max = halfIdx;

  return binarySearchLength(text, min, max);
}
