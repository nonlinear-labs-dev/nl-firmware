#include <proxies/hwui/panel-unit/boled/undo/PositionInTreeLabel.h>
#include <libundo/undo/Transaction.h>

PositionInTreeLabel::PositionInTreeLabel(const Rect &pos)
    : super(pos)
{
}

void PositionInTreeLabel::assignTransaction(UNDO::Transaction *transaction, bool selected, bool current)
{
  if(transaction)
  {
    int above = transaction->countPredecessors();
    int below = transaction->countSuccessorsOnDefaultRoute();
    setText({ to_string(above + 1) + "/" + to_string(above + below + 1) });
  }
}
