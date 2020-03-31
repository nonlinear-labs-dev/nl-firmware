#include <proxies/hwui/controls/ArrowLeft.h>
#include "EditorItem.h"
#include <proxies/hwui/FrameBuffer.h>

bool EditorItem::canEnter()
{
  return true;
}

Rect EditorItem::getDefaultOverlayRect()
{
  auto half = getPosition();
  half.setHeight(half.getHeight() - 2);
  half.setTop(half.getTop() + 1);
  half.setWidth((half.getWidth() / 2) - 2);
  half.setLeft(getPosition().getWidth() / 2);
  return half;
}

Rect EditorItem::getFullRightSideOverlayRect()
{
  auto rightHalf = getPosition();
  rightHalf.setHeight(52);
  rightHalf.setTop(0);
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(getPosition().getWidth() / 2);
  return rightHalf;
}

void EditorItem::doAction()
{
}

bool EditorItem::redraw(FrameBuffer &fb)
{
  auto ret = BasicItem::redraw(fb);
  ret |= redrawEnterIndication(fb);
  return ret;
}

bool EditorItem::redrawEnterIndication(FrameBuffer &buffer)
{
  auto leftRect = getPosition();
  leftRect.setWidth(12);
  leftRect.setLeft(getPosition().getRight() - 12);
  Label s("...", leftRect);
  s.setHighlight(isHighlight());
  return s.redraw(buffer);
}
