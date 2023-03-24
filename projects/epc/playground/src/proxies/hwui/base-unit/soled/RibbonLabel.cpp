#include "RibbonLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/PhysicalControlParameter.h"
#include "proxies/hwui/HWUI.h"
#include "device-settings/SelectedRibbonsSetting.h"
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
  m_signalsChanged.emplace_back(eb->getParameterGroupByID({ "CS", VoiceGroup::Global })
                                    ->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty)));
  m_signalsChanged.emplace_back(eb->getParameterGroupByID({ "MCs", VoiceGroup::Global })
                                    ->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty)));
  m_signalsChanged.emplace_back(eb->getParameterGroupByID({ "MCM", VoiceGroup::Global })
                                    ->onGroupChanged(sigc::mem_fun(this, &RibbonLabel::setDirty)));
  m_signalsChanged.emplace_back(settings->onSettingsChanged(sigc::mem_fun(this, &RibbonLabel::setDirty)));
  m_parameter = eb->findAndCastParameterByID<PhysicalControlParameter>(m_parameterID);

  Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &RibbonLabel::onRibbonSelectionChanged));
}

RibbonLabel::~RibbonLabel() = default;

int RibbonLabel::getXOffset() const
{
  const auto isRibbon1 = m_parameterID == ParameterId { C15::PID::Ribbon_1, VoiceGroup::Global };
  const auto isRibbon2 = m_parameterID == ParameterId { C15::PID::Ribbon_2, VoiceGroup::Global };
  const auto isRibbon3 = m_parameterID == ParameterId { C15::PID::Ribbon_3, VoiceGroup::Global };
  const auto isRibbon4 = m_parameterID == ParameterId { C15::PID::Ribbon_4, VoiceGroup::Global };
  auto [r1Enable, r2Enable, r3Enable, r4Enable] = getRibbonEnabledStates();
  if((isRibbon1 && !r1Enable) || (isRibbon2 && !r2Enable) || (isRibbon3 && !r3Enable) || (isRibbon4 && !r4Enable))
    return 4;

  return Label::getXOffset();
}

void RibbonLabel::onRibbonSelectionChanged(const Setting *s)
{
  if(auto *selectedRibbon = dynamic_cast<const SelectedRibbonsSetting *>(s))
  {
    auto isOneAndTwo = selectedRibbon->get() == SelectedRibbons::Ribbon1_2;

    if(isOneAndTwo)
    {
      if(m_parameterID.getNumber() == C15::PID::Ribbon_3)
        m_parameterID = { C15::PID::Ribbon_1, VoiceGroup::Global };
      else if(m_parameterID.getNumber() == C15::PID::Ribbon_4)
        m_parameterID = { C15::PID::Ribbon_2, VoiceGroup::Global };
    }
    else
    {
      if(m_parameterID.getNumber() == C15::PID::Ribbon_1)
        m_parameterID = { C15::PID::Ribbon_3, VoiceGroup::Global };
      else if(m_parameterID.getNumber() == C15::PID::Ribbon_2)
        m_parameterID = { C15::PID::Ribbon_4, VoiceGroup::Global };
    }
  }

  m_parameter
      = Application::get().getPresetManager()->getEditBuffer()->findAndCastParameterByID<PhysicalControlParameter>(
          m_parameterID);

  setDirty();
}

RibbonLabel::tRibbonEnables RibbonLabel::getRibbonEnabledStates() const
{
  using rIDX = RoutingSettings::tRoutingIndex;
  using aIDX = RoutingSettings::tAspectIndex;

  if(Application::exists())
  {
    auto settings = Application::get().getSettings();
    const auto routingsSetting = settings->getSetting<RoutingSettings>();

    const auto isRibbon1 = m_parameterID == ParameterId { C15::PID::Ribbon_1, VoiceGroup::Global };
    const auto isRibbon2 = m_parameterID == ParameterId { C15::PID::Ribbon_2, VoiceGroup::Global };
    const auto isRibbon3 = m_parameterID == ParameterId { C15::PID::Ribbon_3, VoiceGroup::Global };
    const auto isRibbon4 = m_parameterID == ParameterId { C15::PID::Ribbon_4, VoiceGroup::Global };

    const auto isLocalEnabled = settings->getSetting<GlobalLocalEnableSetting>()->get();
    const auto isRibbon1Enabled = isRibbon1 && routingsSetting->getState(rIDX::Ribbon1, aIDX::LOCAL) && isLocalEnabled;
    const auto isRibbon2Enabled = isRibbon2 && routingsSetting->getState(rIDX::Ribbon2, aIDX::LOCAL) && isLocalEnabled;
    const auto isRibbon3Enabled = isRibbon3 && routingsSetting->getState(rIDX::Ribbon3, aIDX::LOCAL) && isLocalEnabled;
    const auto isRibbon4Enabled = isRibbon4 && routingsSetting->getState(rIDX::Ribbon4, aIDX::LOCAL) && isLocalEnabled;
    return std::make_tuple(isRibbon1Enabled, isRibbon2Enabled, isRibbon3Enabled, isRibbon4Enabled);
  }
  return { false, false, false, false };
}

StringAndSuffix RibbonLabel::getText() const
{
  auto settings = Application::get().getSettings();

  const auto isRibbon1 = m_parameterID == ParameterId { C15::PID::Ribbon_1, VoiceGroup::Global };
  const auto isRibbon2 = m_parameterID == ParameterId { C15::PID::Ribbon_2, VoiceGroup::Global };
  const auto isRibbon3 = m_parameterID == ParameterId { C15::PID::Ribbon_3, VoiceGroup::Global };
  const auto isRibbon4 = m_parameterID == ParameterId { C15::PID::Ribbon_4, VoiceGroup::Global };

  auto [ribbonOneEnabled, ribbonTwoEnabled, ribbonThreeEnabled, ribbonFourEnabled] = getRibbonEnabledStates();
  const auto isRibbonEnabled = ribbonOneEnabled || ribbonTwoEnabled || ribbonThreeEnabled || ribbonFourEnabled;

  if(isRibbonEnabled && m_parameter)
  {
    return crop(m_parameter->getCurrentModulatingMacroControlString());
  }
  else if(!isRibbonEnabled && m_parameter)
  {
    if(isRibbon1)
    {
      return settings->getSetting<RibbonCCMapping<1>>()->getDisplayString();
    }
    else if(isRibbon2)
    {
      return settings->getSetting<RibbonCCMapping<2>>()->getDisplayString();
    }
    else if(isRibbon3)
    {
      return settings->getSetting<RibbonCCMapping<3>>()->getDisplayString();
    }
    else if(isRibbon4)
    {
      return settings->getSetting<RibbonCCMapping<4>>()->getDisplayString();
    }
  }

  return "not assigned";
}

int RibbonLabel::getFontHeight() const
{
  return Label::getFontHeight();
}

std::shared_ptr<Font> RibbonLabel::getFont() const
{
  return Fonts::get().getFont("Emphase-9-Regular", getFontHeight());
}

Glib::ustring RibbonLabel::crop(const Glib::ustring &text) const
{
  auto removeLastChar = [](const auto &s) {
    if(!s.empty())
      return s.substr(0, s.size() - 1);
    return s;
  };

  Glib::ustring ret = text;

  if(getFont()->getStringWidth(ret) > getWidth())
  {
    while(getFont()->getStringWidth(Glib::ustring(ret + "..")) > getWidth())
    {
      ret = removeLastChar(ret);
      if(ret.empty())
        break;
    }

    ret += "..";
  }

  return ret;
}