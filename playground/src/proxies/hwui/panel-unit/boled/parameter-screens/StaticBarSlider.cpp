#include "StaticBarSlider.h"

StaticBarSlider::StaticBarSlider(tControlPositionValue v, bool isBiPolar, const Rect& pos)
    : BarSlider(nullptr, pos)
{
  setHighlight(true);
  setValue(v, isBiPolar);
}
