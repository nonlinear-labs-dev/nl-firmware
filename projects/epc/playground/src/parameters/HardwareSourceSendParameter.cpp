#include <device-settings/Settings.h>
#include <presets/ParameterGroupSet.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include "HardwareSourceSendParameter.h"
#include "use-cases/EditBufferUseCases.h"
#include <presets/EditBuffer.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <parameters/PhysicalControlParameter.h>
#include <xml/Writer.h>

HardwareSourceSendParameter::HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter* sibling,
                                                         const ParameterId& id, const ScaleConverter* converter,
                                                         double def, int coarseDenominator, int fineDenominator,
                                                         Settings* settings)
    : Parameter(pGroup, id, converter, def, coarseDenominator, fineDenominator)
    , m_sibling{sibling}
    , m_settings(settings)
{
  if(m_settings)
  {
    auto local = settings->getSetting<GlobalLocalEnableSetting>();
    local->onChange(sigc::mem_fun(this, &HardwareSourceSendParameter::onLocalChanged));
    auto routings = settings->getSetting<RoutingSettings>();
    routings->onChange(sigc::mem_fun(this, &HardwareSourceSendParameter::onRoutingsChanged));
  }

  if(m_sibling)
  {
    m_sibling->onParameterChanged(sigc::mem_fun(this, &HardwareSourceSendParameter::onSiblingChanged), true);
  }
}

void HardwareSourceSendParameter::setCPFromHwui(UNDO::Transaction* transaction, const tControlPositionValue& cpValue)
{
  Parameter::setCPFromHwui(transaction, cpValue);
  m_lastChangedFromHWUI = true;
}

void HardwareSourceSendParameter::setCPFromWebUI(UNDO::Transaction* transaction, const tControlPositionValue& cpValue)
{
  Parameter::setCPFromWebUI(transaction, cpValue);
  m_lastChangedFromHWUI = false;
}

void HardwareSourceSendParameter::onUnselected()
{
  Parameter::onUnselected();

  if(m_lastChangedFromHWUI && getReturnMode() != ReturnMode::None)
  {
    m_lastChangedFromHWUI = false;
    getValue().setRawValue(Initiator::EXPLICIT_OTHER, getSiblingParameter()->getDefValueAccordingToMode());
    sendToPlaycontroller();
    invalidate();
  }
}

void HardwareSourceSendParameter::onSiblingChanged(const Parameter* sibling)
{
  nltools::Log::error("onSibChanged", getLongName());
  if(auto physicalSrc = dynamic_cast<const PhysicalControlParameter*>(sibling))
  {
    auto newMode = physicalSrc->getReturnMode();
    if(newMode != m_returnMode)
    {
      getValue().setScaleConverter(physicalSrc->getValue().getScaleConverter());
      getValue().setCoarseDenominator(physicalSrc->getValue().getCoarseDenominator());
      getValue().setFineDenominator(physicalSrc->getValue().getFineDenominator());
      getValue().setIsBoolean(physicalSrc->getValue().isBoolean());

      m_returnMode = newMode;
      invalidate();
    }
  }
}

void HardwareSourceSendParameter::sendParameterMessage() const
{
  if(auto eb = getParentEditBuffer())
    eb->getAudioEngineProxy().createAndSendParameterMessage<HardwareSourceSendParameter>(this);
}

void HardwareSourceSendParameter::onLocalChanged(const Setting* setting)
{
  nltools::Log::error("onLocalChanged", getLongName());
  if(auto localSetting = dynamic_cast<const GlobalLocalEnableSetting*>(setting))
  {
    const auto local = localSetting->get();
    if(local != m_localIsEnabled)
    {
      m_localIsEnabled = local;
      calculateIfParameterIsEnabled();
    }
  }
}

void HardwareSourceSendParameter::onRoutingsChanged(const Setting* setting)
{
  nltools::Log::error("onRoutingsChanged", getLongName());
  if(auto routings = dynamic_cast<const RoutingSettings*>(setting))
  {
    const auto state = routings->getState(getIndex(getID()), RoutingSettings::tAspectIndex::LOCAL);
    if(state != m_routingIsEnabled)
    {
      m_routingIsEnabled = state;
      calculateIfParameterIsEnabled();
    }
  }
}

RoutingSettings::tRoutingIndex HardwareSourceSendParameter::getIndex(const ParameterId& id)
{
  using tIdx = RoutingSettings::tRoutingIndex;

  switch(id.getNumber())
  {
    case C15::PID::Bender_Send:
      return tIdx::Bender;
    case C15::PID::Aftertouch_Send:
      return tIdx::Aftertouch;
    case C15::PID::Pedal_1_Send:
      return tIdx::Pedal1;
    case C15::PID::Pedal_2_Send:
      return tIdx::Pedal2;
    case C15::PID::Pedal_3_Send:
      return tIdx::Pedal3;
    case C15::PID::Pedal_4_Send:
      return tIdx::Pedal4;
    case C15::PID::Ribbon_1_Send:
      return tIdx::Ribbon1;
    case C15::PID::Ribbon_2_Send:
      return tIdx::Ribbon2;
  }
  nltools_assertNotReached();
}

nlohmann::json HardwareSourceSendParameter::serialize() const
{
  auto param = Parameter::serialize();
  param.push_back({"is-enabled", isLocalEnabled() });
  param.push_back({"return-mode", static_cast<int>(m_returnMode) });
  return param;
}

void HardwareSourceSendParameter::writeDocProperties(Writer& writer,
                                                     UpdateDocumentContributor::tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("local-enabled", std::to_string(isLocalEnabled()));
  writer.writeTextElement("return-mode", std::to_string(static_cast<int>(m_returnMode)));
}

bool HardwareSourceSendParameter::isLocalEnabled() const
{
  return m_isEnabled;
}

bool HardwareSourceSendParameter::lockingEnabled() const
{
  return false;
}

ReturnMode HardwareSourceSendParameter::getReturnMode() const
{
  return m_sibling->getReturnMode();
}

Layout* HardwareSourceSendParameter::createLayout(FocusAndMode focusAndMode) const
{
  return m_sibling->createLayout(focusAndMode);
}

PhysicalControlParameter* HardwareSourceSendParameter::getSiblingParameter() const
{
  return m_sibling;
}

void HardwareSourceSendParameter::calculateIfParameterIsEnabled()
{
  nltools::Log::error("calcIfEnabled", getLongName());
  auto oldState = m_isEnabled;
  m_isEnabled = m_routingIsEnabled && m_localIsEnabled;
  if(oldState != m_isEnabled)
  {
    if(m_isEnabled )
    {
      if(auto eb = getParentEditBuffer())
      {
        EditBufferUseCases useCase(*eb);
        useCase.selectParameter(getSiblingParameter(), true);
      }
    }

    nltools::Log::error("invalidating", getLongName());
    invalidate();
  }
}
