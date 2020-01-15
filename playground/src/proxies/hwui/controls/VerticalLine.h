#pragma once

#include "Control.h"

class VerticalLine : public Control
{
 private:
  typedef Control super;

 public:
  VerticalLine(const Rect& rect);

  bool redraw(FrameBuffer& fb);
};
