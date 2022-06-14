#include "MCAmountButton.h"
#include "parameters/ModulateableParameter.h"
#include "groups/MasterGroup.h"
#include "groups/VoiceGroupMasterGroup.h"
#include "groups/ScaleGroup.h"
#include "groups/UnisonGroup.h"
#include "groups/SplitParameterGroups.h"
#include <proxies/hwui/buttons.h>
#include <parameters/unison-parameters/ModulateableUnisonParameterWithUnusualModUnit.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>

MCAmountButton::MCAmountButton(Buttons id)
    : super("MC Amt", id)
{
}

MCAmountButton::~MCAmountButton() = default;

void MCAmountButton::update(const Parameter* parameter)
{
  auto getSpecialParameter = [](const Parameter* param) -> const ModulateableParameter*
  {
    if(const auto* u = dynamic_cast<const ModulateableUnisonParameterWithUnusualModUnit*>(param))
      return u;
    else if(const auto* a = dynamic_cast<const ModulateableMonoParameter*>(param))
      return a;
    else if(param != nullptr)
    {
      auto group = param->getParentGroup();
      if(dynamic_cast<const MasterGroup*>(group) || dynamic_cast<const VoiceGroupMasterGroup*>(group)
         || dynamic_cast<const UnisonGroup*>(group)
         || dynamic_cast<const SplitParameterGroups*>(group) || dynamic_cast<const VoiceGroupMasterGroup*>(group))
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
    if(p->getModulationSource() == MacroControls::NONE)
      setText(StringAndSuffix::empty());
    else
      setText(StringAndSuffix { std::string("MC Amt") + (p->isModAmountChanged() ? "*" : "") });
  }
  else
  {
    setText(StringAndSuffix::empty());
  }
}
