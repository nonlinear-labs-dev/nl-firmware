#include "Overlay.h"
#include <proxies/hwui/FrameBuffer.h>

Overlay::Overlay(const Rect &pos)
    : super(pos)
{
}

void Overlay::drawBackground(FrameBuffer &fb)
{
}

void Overlay::setBackgroundColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

bool Overlay::isTransparent() const
{
  return true;
}
