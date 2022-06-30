#include <Application.h>
#include "UndoIndicator.h"
#include <http/UndoScope.h>

UndoIndicator::UndoIndicator(const Rect &pos)
    : Label("\uE20E", Rect(pos.getLeft(), pos.getTop(), 8, 7))
{
  Application::get().getUndoScope()->onUndoScopeChanged(mem_fun(this, &UndoIndicator::setDirty));
}

bool UndoIndicator::redraw(FrameBuffer &fb)
{
  if(Application::get().getUndoScope()->canRedo())
  {
    Label::redraw(fb);
  }
  return true;
}

int UndoIndicator::getZPriority() const
{
  return 1;
}
