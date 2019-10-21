#include "ParameterValueLabel.h"
#include <parameters/Parameter.h>

ParameterValueLabel::ParameterValueLabel(const Parameter *param, const Rect &pos)
    : Label(pos)
{
  if(param)
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterValueLabel::updateText));
}

ParameterValueLabel::~ParameterValueLabel()
{
  m_connection.disconnect();
}

void ParameterValueLabel::updateText(const Parameter *param)
{
  setText(param->getDisplayString());
}

void ParameterValueLabel::updateParameter(const Parameter *param)
{
  m_connection.disconnect();

  if(param)
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterValueLabel::updateText));
}
