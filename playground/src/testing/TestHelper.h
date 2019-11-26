#pragma once
#include <libundo/undo/TransactionCreationScope.h>
#include <memory>
#include <presets/PresetManager.h>
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

  namespace MainLoop
  {
    namespace detail
    {
      inline Glib::RefPtr<MainContext> getMainLoop()
      {
        return Application::get().getMainContext();
      }
    }

    inline bool hasPending()
    {
      return detail::getMainLoop()->pending();
    }

    inline bool iterateLoopOnce()
    {
      auto mainloop = detail::getMainLoop();
      return mainloop->iteration(true);
    }
  }
}
