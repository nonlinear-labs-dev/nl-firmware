#include <proxies/hwui/HWUIEnums.h>
#include "ControlWithChildren.h"
#include <proxies/hwui/FrameBuffer.h>

ControlWithChildren::ControlWithChildren(const Rect &pos)
    : super(pos)
{
}

ControlWithChildren::~ControlWithChildren() = default;

void ControlWithChildren::setDirty()
{
  Control::setDirty();
}

bool ControlWithChildren::isHighlight() const
{
  return Control::isHighlight();
}

void ControlWithChildren::setHighlight(bool isHighlight)
{
  Control::setHighlight(isHighlight);
  ControlOwner::setHighlight(isHighlight);
}

bool ControlWithChildren::redraw(FrameBuffer &fb)
{
  if(!isTransparent())
    Control::drawBackground(fb);

  auto clip = fb.clipRespectingOffset(getPosition());

  if(clip.isEmpty())
    return false;

  auto offset = fb.offset(getPosition().getPosition());

  return ControlOwner::redraw(fb);
}

void ControlWithChildren::drawBackground(FrameBuffer &fb)
{
}

bool ControlWithChildren::isTransparent() const
{
  return false;
}

void ControlWithChildren::setVisible(bool b)
{
  for(auto &c : getControls())
  {
    c->setVisible(b);
  }

  Control::setVisible(b);
}
