#include <proxies/hwui/panel-unit/boled/undo/UndoList.h>
#include <Application.h>
#include <http/UndoScope.h>
#include <proxies/hwui/FrameBuffer.h>
#include "UndoListEntry.h"
#include <libundo/undo/Transaction.h>

UndoList::UndoList(const Rect &pos)
    : super(pos)
{
  for(int i = 0; i < 4; i++)
  {
    addControl(new UndoListEntry(Rect(0, i * 16, pos.getWidth(), 16)));
  }
}

void UndoList::assignTransactions(UNDO::Transaction *tip)
{
  auto current = Application::get().getUndoScope()->getUndoTransaction();
  auto walker = tip;

  auto it = getControls().rbegin();

  if(auto lastToShow = walker->getDefaultRedoRoute())
  {
    walker = lastToShow;
  }
  else if(auto entry = std::dynamic_pointer_cast<UndoListEntry>(*it))
  {
    entry->assignTransaction(nullptr, false, false);
    it++;
  }

  for(; it != getControls().rend(); it++)
  {
    auto ctrl = *it;

    if(auto entry = std::dynamic_pointer_cast<UndoListEntry>(ctrl))
    {
      entry->assignTransaction(walker, walker == tip, walker == current);

      if(walker)
        walker = walker->getPredecessor();
    }
  }
}

bool UndoList::redraw(FrameBuffer &fb)
{
  super::redraw(fb);

  fb.setColor(FrameBufferColors::C128);
  const Rect &r = getPosition();
  fb.drawRect(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight());

  return true;
}
