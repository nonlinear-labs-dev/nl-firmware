#pragma once

#include <proxies/hwui/controls/BarSlider.h>

class StaticBarSlider : public BarSlider
{
 public:
  StaticBarSlider(tControlPositionValue v, bool isBiPolar, const Rect &pos);
};
