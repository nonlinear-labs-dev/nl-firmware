#pragma once
#include <Application.h>
#include <parameters/ModulateableParameter.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

class LambdaFactory
{
 public:
  static auto setCurrentModulatedParamAmountToZero()
  {
    return []() {
      auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
      if(auto modulatedParam = dynamic_cast<ModulateableParameter *>(editBuffer->getSelected()))
      {
        auto &undoScope = modulatedParam->getUndoScope();
        auto scope = undoScope.startContinuousTransaction(modulatedParam, "Set MC Amount for '%0'",
                                                          modulatedParam->getGroupAndParameterName());
        auto transaction = scope->getTransaction();
        modulatedParam->setModulationAmount(transaction, 0);
        return true;
      }
      return false;
    };
  };

  static auto setCurrentParameterToDefault()
  {
    return []() {
      auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
      if(auto parameter = editBuffer->getSelected())
      {
        parameter->setDefaultFromHwui();
        return true;
      }
      return false;
    };
  };
};