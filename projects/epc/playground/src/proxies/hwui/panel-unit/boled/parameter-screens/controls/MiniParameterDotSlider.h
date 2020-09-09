#pragma once

#include "proxies/hwui/controls/DotSlider.h"

class MiniParameterDotSlider : public DotSlider
{
 private:
  typedef DotSlider super;

 public:
  MiniParameterDotSlider(Parameter *param, const Rect &rect);

  bool redraw(FrameBuffer &fb) override;

 private:
  bool hasBorder() const override;
};
