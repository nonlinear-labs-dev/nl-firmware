#include "BasicItem.h"
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/CaptionLabel.h>
#include <proxies/hwui/FrameBuffer.h>

bool BasicItem::canEnter()
{
  return false;
}

bool BasicItem::redraw(FrameBuffer& fb)
{
  auto ret = false;
  ret |= ControlWithChildren::redraw(fb);
  ret |= drawHighlightBorder(fb);
  return ret;
}

bool BasicItem::drawHighlightBorder(FrameBuffer& fb)
{
  if(!isHighlight())
    return false;

  fb.setColor(FrameBufferColors::C103);
  fb.drawRect(getPosition());
  return true;
}

void BasicItem::init()
{
  auto leftHalf = getPosition();
  leftHalf.setLeft(1);
  leftHalf.setWidth((leftHalf.getWidth() / 2) - 1);

  addControl(new CaptionLabel<LeftAlignedLabel>(m_caption, leftHalf, true));
}
