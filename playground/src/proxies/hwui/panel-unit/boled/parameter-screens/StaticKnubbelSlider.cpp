#include "StaticKnubbelSlider.h"

StaticKnubbelSlider::StaticKnubbelSlider(tControlPositionValue value, bool biPolar, Rect pos)
    : DotSlider(nullptr, pos)
{
  setHighlight(true);
  setValue(value, biPolar);
}

void StaticKnubbelSlider::setValueExplicit(tControlPositionValue value, bool biPolar)
{
  setValue(value, biPolar);
}
