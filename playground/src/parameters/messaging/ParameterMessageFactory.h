
#include <parameters/Parameter.h>
#include <nltools/messaging/Message.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>

class ParameterMessageFactory {
public:
  static nltools::msg::UnmodulateableParameterChangedMessage createMessage(const Parameter* param);
  static nltools::msg::ModulateableParameterChangedMessage createMessage(const ModulateableParameter* param);
  static nltools::msg::MacroControlChangedMessage createMessage(const MacroControlParameter* param);
  static nltools::msg::HWAmountChangedMessage createMessage(const ModulationRoutingParameter* param);
  static nltools::msg::HWSourceChangedMessage createMessage(const PhysicalControlParameter* param);
};

