#pragma once

#include "MiniParameterBarSlider.h"

class ScaleGroupMiniParameterBarSlider : public MiniParameterBarSlider
{
  using super = MiniParameterBarSlider;

 public:
  ScaleGroupMiniParameterBarSlider(Parameter *param, const Rect &rect);
  ~ScaleGroupMiniParameterBarSlider() override;

  tDisplayValue getDrawValue() const override;
  void setSliderColor(FrameBuffer &fb) override;
  void drawBackground(FrameBuffer &fb) override;
  bool redraw(FrameBuffer &fb) override;

 private:
  sigc::connection m_connection;
};
