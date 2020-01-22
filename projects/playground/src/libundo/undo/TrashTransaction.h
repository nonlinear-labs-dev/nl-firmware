#pragma once

#include "Transaction.h"

namespace UNDO
{
  class TrashTransaction : public Transaction
  {
   public:
    TrashTransaction();

    void addCommand(tCommandPtr cmd) override;
    void addSimpleCommand(ActionCommand::tAction doAndRedo, ActionCommand::tAction undo) override;
    void addSimpleCommand(ActionCommand::tAction doRedoUndo) override;
  };
}
