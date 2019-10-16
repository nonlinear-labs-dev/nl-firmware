#include "Rectangle.h"

Rectangle::Rectangle(const Rect& r, FrameBuffer::Colors color)
    : Control(r)
    , m_color{ color }
{
}

bool Rectangle::redraw(FrameBuffer& fb)
{
  fb.setColor(m_color);
  fb.fillRect(getPosition());
  return Control::redraw(fb);
}
