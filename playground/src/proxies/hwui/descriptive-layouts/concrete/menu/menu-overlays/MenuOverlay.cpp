#include "MenuOverlay.h"
#include <proxies/hwui/FrameBuffer.h>

MenuOverlay::MenuOverlay(const Rect& r)
    : ControlWithChildren(r)
{
}

bool MenuOverlay::redraw(FrameBuffer& fb)
{
  auto ret = ControlWithChildren::redraw(fb);
  fb.setColor(FrameBuffer::C204);
  auto pos = getPosition();
  pos.setHeight(pos.getHeight() + 2);
  pos.setTop(pos.getTop() - 1);
  pos.setWidth(pos.getWidth() + 2);
  pos.draw(fb);
  return ret;
}
