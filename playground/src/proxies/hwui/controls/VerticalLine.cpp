#include "VerticalLine.h"
#include <proxies/hwui/FrameBuffer.h>

VerticalLine::VerticalLine(const Rect &rect)
    : super(rect)
{
}

bool VerticalLine::redraw(FrameBuffer &fb)
{
  fb.setColor(FrameBufferColors::C128);

  Rect r = getPosition();
  Point c = r.getCenter();

  fb.drawVerticalLine(c.getX(), r.getY(), r.getHeight());
  return true;
}
