#pragma once
#include <libundo/undo/TransactionCreationScope.h>
#include <memory>
#include <presets/PresetManager.h>

namespace TestHelper
{
  inline auto createTestScope() -> std::unique_ptr<UNDO::TransactionCreationScope>
  {
    return std::move(Application::get().getPresetManager()->getUndoScope().startTestTransaction());
  }
}