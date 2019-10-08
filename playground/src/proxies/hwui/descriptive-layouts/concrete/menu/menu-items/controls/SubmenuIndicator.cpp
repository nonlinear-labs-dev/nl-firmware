#include <proxies/hwui/controls/SymbolLabel.h>
#include "SubmenuIndicator.h"

static Rect getArrowLineRect(const Rect &full)
{
  auto temp = full;
  temp.setWidth(full.getWidth() / 2.0);
  temp.setLeft(full.getLeft() + temp.getWidth());
  return temp;
}

static Rect getArrowHeadRect(const Rect &r)
{
  auto temp = r;
  temp.setWidth(r.getWidth() / 2.0);
  temp.setLeft(r.getLeft());
  return temp;
}

SubmenuIndicator::SubmenuIndicator(const Rect &rect)
    : Control(rect)
    , m_label("e", getArrowHeadRect(getPosition()))
{
}

bool SubmenuIndicator::redraw(FrameBuffer &fb)
{
  if(isHighlight())
    m_label.setFontColor(FrameBuffer::Colors::C255);
  else
    m_label.setFontColor(FrameBuffer::Colors::C179);

  m_label.redraw(fb);
  auto pos = getArrowLineRect(m_label.getPosition());

  if(isHighlight())
    fb.setColor(FrameBuffer::Colors::C255);
  else
    fb.setColor(FrameBuffer::Colors::C179);

  fb.drawHorizontalLine(pos.getLeft() + 2, pos.getCenter().getY() + 1, 3);
  fb.drawVerticalLine(pos.getLeft() + 4, pos.getCenter().getY() - 2, 4);
  return true;
}
