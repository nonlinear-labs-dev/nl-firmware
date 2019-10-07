#include "ParameterMessageFactory.h"

namespace ParameterMessageFactory
{

  namespace detail
  {
    nltools::msg::UnmodulateableParameterChangedMessage createMessage(const Parameter *param)
    {
      return nltools::msg::UnmodulateableParameterChangedMessage{ param->getID(), param->getControlPositionValue(),
                                                                  param->isLocked() };
    }

    nltools::msg::ModulateableParameterChangedMessage createMessage(const ModulateableParameter *param)
    {
      auto range = param->getModulationRange(false);
      return nltools::msg::ModulateableParameterChangedMessage{ param->getID(),
                                                                param->getControlPositionValue(),
                                                                param->getModulationSource(),
                                                                param->getModulationAmount(),
                                                                range.second,
                                                                range.first,
                                                                param->isLocked() };
    }

    nltools::msg::MacroControlChangedMessage createMessage(const MacroControlParameter *param)
    {
      return nltools::msg::MacroControlChangedMessage{ param->getID(), param->getControlPositionValue(), 0.0,
                                                       param->isLocked() };
    }

    nltools::msg::HWAmountChangedMessage createMessage(const ModulationRoutingParameter *param)
    {
      return nltools::msg::HWAmountChangedMessage{ param->getID(), param->getControlPositionValue(),
                                                   param->isLocked() };
    }

    nltools::msg::HWSourceChangedMessage createMessage(const PhysicalControlParameter *param)
    {
      return nltools::msg::HWSourceChangedMessage{ param->getID(), param->getControlPositionValue(),
                                                   param->getReturnMode(), param->isLocked() };
    }
  }
}
