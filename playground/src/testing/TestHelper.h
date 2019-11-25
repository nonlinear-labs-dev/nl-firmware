#pragma once
#include <libundo/undo/TransactionCreationScope.h>
#include <memory>
#include <presets/PresetManager.h>
#include <parameters/Parameter.h>

namespace TestHelper
{
  inline PresetManager* getPresetManager()
  {
    return Application::get().getPresetManager();
  }

  inline EditBuffer* getEditBuffer()
  {
    return getPresetManager()->getEditBuffer();
  }

  inline std::unique_ptr<UNDO::TransactionCreationScope> createTestScope()
  {
    return std::move(getPresetManager()->getUndoScope().startTestTransaction());
  }

  inline void forceParameterChange(UNDO::Transaction* transaction, Parameter* param)
  {
    auto currentValue = param->getControlPositionValue();

    auto incNext = param->getValue().getNextStepValue(1, {});
    auto decNext = param->getValue().getNextStepValue(-1, {});

    if(incNext != currentValue)
      param->setCPFromHwui(transaction, incNext);
    else if(decNext != currentValue)
      param->setCPFromHwui(transaction, decNext);
  }
}
