#pragma once
#include <parameter_list.h>
#include <parameter_descriptor.h>
#include <parameter_declarations.h>
#include <nltools/messaging/Message.h>


constexpr auto NUM_HW = static_cast<int>(C15::Parameters::Hardware_Sources::_LENGTH_);
constexpr auto NUM_LOCAL_UNMOD = static_cast<int>(C15::Parameters::Local_Unmodulateables::_LENGTH_);
constexpr auto NUM_LOCAL_MOD = static_cast<int>(C15::Parameters::Local_Modulateables::_LENGTH_);
constexpr auto NUM_GLOBAL_UNMOD = static_cast<int>(C15::Parameters::Global_Unmodulateables::_LENGTH_);
constexpr auto NUM_GLOBAL_MOD = static_cast<int>(C15::Parameters::Global_Modulateables::_LENGTH_);
constexpr auto NUM_HWAMT = static_cast<int>(C15::Parameters::Hardware_Amounts::_LENGTH_);
constexpr auto NUM_MC = static_cast<int>(C15::Parameters::Macro_Controls::_LENGTH_);
constexpr auto NUM_MC_Times = static_cast<int>(C15::Parameters::Macro_Times::_LENGTH_);

struct GenericSinglePresetMessage {
  constexpr static nltools::msg::MessageType getType() {
    return nltools::msg::MessageType::SinglePreset;
  }

  std::array<nltools::msg::ParameterGroups::Parameter, NUM_LOCAL_UNMOD> m_localUnmodulateable;
  std::array<nltools::msg::ParameterGroups::ModulateableParameter, NUM_LOCAL_MOD> m_localModulateable;
  std::array<nltools::msg::ParameterGroups::Parameter, NUM_GLOBAL_UNMOD> m_globalUnmodulateable;
  ///ETC.....
};