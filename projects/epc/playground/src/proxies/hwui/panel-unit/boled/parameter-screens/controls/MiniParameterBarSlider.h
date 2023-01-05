#pragma once

#include "proxies/hwui/controls/BarSlider.h"

class MiniParameterBarSlider : public BarSlider
{
 private:
  typedef BarSlider super;

 public:
  MiniParameterBarSlider(Parameter* param, const Rect& rect);

  bool redraw(FrameBuffer& fb) override;
  void setSliderColor(FrameBuffer& fb) override;

 private:
  bool hasBorder() const override;
  bool drawCenterMark() const override;
};
