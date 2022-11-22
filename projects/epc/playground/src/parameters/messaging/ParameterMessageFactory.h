#pragma once
#include <ParameterMessages.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/HardwareSourceSendParameter.h>

namespace ParameterMessageFactory
{

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
    // safety proposal: id pointing to non hw-source descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non hw-source-send descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Hardware_Amount>(const ModulationRoutingParameter *param)
{
    auto ret = nltools::msg::HardwareAmountParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    // safety proposal: id pointing to non hw-amount descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Macro_Control>(const MacroControlParameter *param)
{
    auto ret = nltools::msg::MacroControlParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    // safety proposal: id pointing to non macro-control descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
    return ret;
}

template<>
inline auto ParameterMessageFactory::createParameterChangedMessage
<C15::Descriptors::ParameterType::Macro_Time>(const Parameter *param)
{
    auto ret = nltools::msg::MacroTimeParameterChangedMessage {};
    ret.m_id = (C15::PID::ParameterID) param->getID().getNumber();
    ret.m_controlPosition = param->getControlPositionValue();
    // safety proposal: id pointing to non macro-time descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non global-modulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non global-unmodulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non local-modulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non local-unmodulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non polyphonic-modulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non polyphonic-unmodulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non monophonic-modulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
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
    // safety proposal: id pointing to non monophonic-unmodulateable descriptor will fail
    nltools_assertAlways(ret.validateParameterType());
    return ret;
}
