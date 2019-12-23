#pragma once
#include <libundo/undo/TransactionCreationScope.h>
#include <memory>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/Parameter.h>

namespace TestHelper
{
  namespace floating
  {
    template <typename T> inline bool equals(T first, T second)
    {
      constexpr auto epsilon = std::numeric_limits<T>::epsilon();
      return std::abs(first - second) <= epsilon;
    }

    template <typename T> inline bool differs(T first, T second)
    {
      return !equals(first, second);
    }
  }

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

    if(floating::differs(incNext, currentValue))
      param->setCPFromHwui(transaction, incNext);
    else if(floating::differs(decNext, currentValue))
      param->setCPFromHwui(transaction, decNext);
    else
      nltools_detailedAssertAlways(false, "Unable to change Parameter Value in either direction");
  }

  template <typename tCB> inline void forEachParameter(const tCB& cb, EditBuffer* eb)
  {
    for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
      for(auto& g : eb->getParameterGroups(vg))
        for(auto& p : g->getParameters())
          cb(p);
  }
}
