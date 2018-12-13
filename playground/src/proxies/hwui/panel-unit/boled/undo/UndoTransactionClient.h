#pragma once

#include <libundo/undo/Transaction.h>

class UndoTransactionClient
{
 public:
  virtual void assignTransaction(UNDO::Transaction *transaction, bool selected, bool current) = 0;
};
