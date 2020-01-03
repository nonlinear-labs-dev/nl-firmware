#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <libundo/undo/Transaction.h>

class UndoList : public ControlWithChildren
{
 private:
  typedef ControlWithChildren super;

 public:
  UndoList(const Rect &pos);

  void assignTransactions(UNDO::Transaction *tip);
  bool redraw(FrameBuffer &fb) override;
};
