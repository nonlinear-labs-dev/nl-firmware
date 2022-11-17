#include "DottedLine.h"
#include <proxies/hwui/FrameBuffer.h>

DottedLine::DottedLine(const Rect &rect)
    : super(rect)
{
}

bool DottedLine::redraw(FrameBuffer &fb)
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C204);

  Rect r = getPosition();
  fb.drawDottedLine(r.getLeft(), r.getY(), r.getWidth());
  return true;
}
