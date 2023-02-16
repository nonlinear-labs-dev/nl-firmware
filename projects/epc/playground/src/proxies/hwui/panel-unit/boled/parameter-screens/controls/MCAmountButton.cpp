#include "MCAmountButton.h"
#include "parameters/ModulateableParameter.h"
#include "groups/MasterGroup.h"
#include "groups/VoiceGroupMasterGroup.h"
#include "parameters/unison-parameters/UnmodulateableUnisonParameter.h"
#include <proxies/hwui/buttons.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <parameters/ParameterFactory.h>

MCAmountButton::MCAmountButton(Buttons id)
    : super("MC Amt", id)
{
}

MCAmountButton::~MCAmountButton() = default;

void MCAmountButton::update(const Parameter* parameter)
{
  auto getSpecialParameter = [](const Parameter* param) -> const ModulateableParameter* {
    if(const auto* u = dynamic_cast<const ModulateableUnisonParameter*>(param))
      return u;
    else if(const auto* a = dynamic_cast<const ModulateableMonoParameter*>(param))
      return a;
    else if(param != nullptr)
    {
      const auto isMaster = ParameterFactory::isMasterParameter(param);
      const auto isPartMaster = ParameterFactory::isVoiceGroupMasterParameter(param->getID());
      const auto isUnison = ParameterFactory::isUnisonParameter(param);
      const auto isSplitParameter = ParameterFactory::isSplitPoint(param);

      if(isMaster || isPartMaster || isUnison || isSplitParameter)
        return dynamic_cast<const ModulateableParameter*>(param);
    }
    return nullptr;
  };

  if(const auto* u = getSpecialParameter(parameter))
  {
    if(u->getModulationSource() == MacroControls::NONE)
    {
      setText(StringAndSuffix { "back.." });
      return;
    }
  }

  if(const auto* p = dynamic_cast<const ModulateableParameter*>(parameter))
  {
    if(p->getModulationSource() == MacroControls::NONE || p->isDisabled())
      setText(StringAndSuffix::empty());
    else
      setText(StringAndSuffix { std::string("MC Amt") + (p->isModAmountChanged() ? "*" : "") });
  }
  else
  {
    setText(StringAndSuffix::empty());
  }
}
