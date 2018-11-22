#include "StaticKnubbelSlider.h"

StaticKnubbelSlider::StaticKnubbelSlider(Parameter* parameter, Rect pos)
    : DotSlider(parameter, pos)
{
  setHighlight(true);
  setValue(parameter->getControlPositionValue(), parameter->isBiPolar());
}

void StaticKnubbelSlider::onParamValueChanged(const Parameter* param)
{
}

void StaticKnubbelSlider::setValueExplicit(tControlPositionValue value)
{
  setValue(value, getParameter()->isBiPolar());
}
