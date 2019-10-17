#pragma once
#include "Control.h"

class Rectangle : public Control
{
 public:
  Rectangle(const Rect& r, FrameBuffer::Colors color);
  bool redraw(FrameBuffer& fb) override;

 private:
  FrameBuffer::Colors m_color;
};
