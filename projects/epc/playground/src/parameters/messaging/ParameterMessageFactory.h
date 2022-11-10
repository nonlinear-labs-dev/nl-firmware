#pragma once
#include <parameters/Parameter.h>
//#include <nltools/messaging/Message.h>
#include <ParameterMessages.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/HardwareSourceSendParameter.h>

namespace ParameterMessageFactory
{
  // todo: deprecate
  namespace detail
  {
    nltools::msg::UnmodulateableParameterChangedMessage createMessage(const Parameter *param);
    nltools::msg::ModulateableParameterChangedMessage createMessage(const ModulateableParameter *param);
    nltools::msg::MacroControlChangedMessage createMessage(const MacroControlParameter *param);
    nltools::msg::HWAmountChangedMessage createMessage(const ModulationRoutingParameter *param);
    nltools::msg::HWSourceChangedMessage createMessage(const PhysicalControlParameter *param);
    nltools::msg::HWSourceSendChangedMessage createMessage(const HardwareSourceSendParameter* param);
  };

  template <typename tParameter> auto createMessage(const tParameter *parameter)
  {
    return detail::createMessage(parameter);
  }

  // todo: use
  template<C15::Descriptors::ParameterType, typename T>
  auto createParameterChangedMessage(const T*);

};

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Hardware_Source>(const PhysicalControlParameter *param)
{
    auto ret = nltools::msg::HardwareSourceParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_returnMode = param->getReturnMode();
    ret.m_isLocalEnabled = param->isLocalEnabled();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Display_Parameter>(const HardwareSourceSendParameter *param)
{
    auto ret = nltools::msg::HardwareSourceSendParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_returnMode = param->getReturnMode();
    ret.m_siblingId = (C15::PID::ParameterID) param->getSiblingParameter()->getID().getNumber();
    ret.m_isLocalEnabled = param->isLocalEnabled();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Hardware_Amount>(const ModulationRoutingParameter *param)
{
    auto ret = nltools::msg::HardwareAmountParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Macro_Control>(const MacroControlParameter *param)
{
    auto ret = nltools::msg::MacroControlParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Macro_Time>(const Parameter *param)
{
    auto ret = nltools::msg::MacroTimeParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Global_Modulateable>(const ModulateableParameter *param)
{
    const auto range = param->getModulationRange(false);
    auto ret = nltools::msg::GlobalModulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_macro = param->getModulationSource();
    ret.m_modulationAmount = param->getModulationAmount();
    ret.m_modulationUpper = range.second;
    ret.m_modulationLower= range.first;
    // voicegroup is global (static constexpr)
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Global_Unmodulateable>(const Parameter *param)
{
    auto ret = nltools::msg::GlobalUnmodulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    // voicegroup is global (static constexpr)
    return ret;
}

// todo: deprecate
template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Local_Modulateable>(const ModulateableParameter *param)
{
    const auto range = param->getModulationRange(false);
    auto ret = nltools::msg::LocalModulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_macro = param->getModulationSource();
    ret.m_modulationAmount = param->getModulationAmount();
    ret.m_modulationUpper = range.second;
    ret.m_modulationLower= range.first;
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}

// todo: deprecate
template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Local_Unmodulateable>(const Parameter *param)
{
    auto ret = nltools::msg::LocalUnmodulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Polyphonic_Modulateable>(const ModulateableParameter *param)
{
    const auto range = param->getModulationRange(false);
    auto ret = nltools::msg::PolyphonicModulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_macro = param->getModulationSource();
    ret.m_modulationAmount = param->getModulationAmount();
    ret.m_modulationUpper = range.second;
    ret.m_modulationLower= range.first;
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Polyphonic_Unmodulateable>(const Parameter *param)
{
    auto ret = nltools::msg::PolyphonicUnmodulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Monophonic_Modulateable>(const ModulateableParameter *param)
{
    const auto range = param->getModulationRange(false);
    auto ret = nltools::msg::MonophonicModulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_macro = param->getModulationSource();
    ret.m_modulationAmount = param->getModulationAmount();
    ret.m_modulationUpper = range.second;
    ret.m_modulationLower= range.first;
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Monophonic_Unmodulateable>(const Parameter *param)
{
    auto ret = nltools::msg::MonophonicUnmodulateableParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    ret.m_voiceGroup = param->getID().getVoiceGroup();
    return ret;
}
