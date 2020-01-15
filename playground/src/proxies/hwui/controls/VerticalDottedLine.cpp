#include "VerticalDottedLine.h"
#include <proxies/hwui/FrameBuffer.h>

VerticalDottedLine::VerticalDottedLine(const Rect &rect)
    : super(rect)
{
}

bool VerticalDottedLine::redraw(FrameBuffer &fb)
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C204);

  Rect r = getPosition();
  Point c = r.getCenter();

  for(int i = r.getTop(); i <= r.getBottom(); i += 2)
    fb.setPixel(r.getX(), i);

  return true;
}
