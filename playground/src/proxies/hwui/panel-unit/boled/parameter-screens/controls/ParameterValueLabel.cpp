#include "ParameterValueLabel.h"
#include <parameters/Parameter.h>

ParameterValueLabel::ParameterValueLabel(const Parameter *param, const Rect &pos)
    : Label(pos)
{
  if(param)
    param->onParameterChanged(mem_fun(this, &ParameterValueLabel::updateText));
}

void ParameterValueLabel::updateText(const Parameter *param)
{
  setText(param->getDisplayString());
}
