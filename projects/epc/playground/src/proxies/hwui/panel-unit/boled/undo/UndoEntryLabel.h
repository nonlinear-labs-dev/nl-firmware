#pragma once

#include <proxies/hwui/controls/Label.h>
#include "UndoTransactionClient.h"

class UndoEntryLabel : public Label, public UndoTransactionClient
{
 private:
  typedef Label super;

 public:
  UndoEntryLabel(const Rect &pos);
  virtual ~UndoEntryLabel();

  virtual void assignTransaction(UNDO::Transaction *transaction, bool selected, bool current) override;
  virtual void setBackgroundColor(FrameBuffer &fb) const override;
};
