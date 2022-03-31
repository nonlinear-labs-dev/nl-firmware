#pragma once
#include <parameters/Parameter.h>
#include <nltools/messaging/Message.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/HardwareSourceSendParameter.h>

namespace ParameterMessageFactory
{
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
};
