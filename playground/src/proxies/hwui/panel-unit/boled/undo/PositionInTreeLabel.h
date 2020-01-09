#pragma once

#include <proxies/hwui/controls/Label.h>
#include "UndoTransactionClient.h"

class PositionInTreeLabel : public Label, public UndoTransactionClient
{
 private:
  typedef Label super;

 public:
  explicit PositionInTreeLabel(const Rect &pos);
  void assignTransaction(UNDO::Transaction *transaction, bool selected, bool current) override;
};
