#include "EmptyBankLabel.h"
#include <proxies/hwui/FrameBuffer.h>

EmptyBankLabel::EmptyBankLabel(const Rect &pos)
    : super("empty", pos)
{
}

EmptyBankLabel::~EmptyBankLabel()
{
}

void EmptyBankLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C179);
}

bool EmptyBankLabel::redraw(FrameBuffer &fb)
{
  super::redraw(fb);
  fb.setColor(FrameBufferColors::C179);
  const Rect &r = getPosition();
  fb.drawRect(r.getLeft() + 1, r.getTop(), r.getWidth() - 2, r.getHeight());
  return true;
}
