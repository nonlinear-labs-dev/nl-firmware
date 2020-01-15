#pragma once

#include "Control.h"
#include <proxies/hwui/HWUIEnums.h>

class Rectangle : public Control
{
 public:
  Rectangle(const Rect& r, FrameBufferColors color);
  bool redraw(FrameBuffer& fb) override;

 private:
  FrameBufferColors m_color;
};
