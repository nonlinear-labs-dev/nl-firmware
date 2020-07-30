#include <proxies/hwui/panel-unit/boled/undo/UndoEntryLabel.h>
#include <proxies/hwui/FrameBuffer.h>

UndoEntryLabel::UndoEntryLabel(const Rect &pos)
    : super(pos)
{
}

UndoEntryLabel::~UndoEntryLabel()
{
}

void UndoEntryLabel::assignTransaction(UNDO::Transaction *transaction, bool selected, bool current)
{
  setHighlight(current);
}

void UndoEntryLabel::setBackgroundColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    super::setBackgroundColor(fb);
}
