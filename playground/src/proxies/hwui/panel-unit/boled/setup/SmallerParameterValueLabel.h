#pragma once
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>

class SmallerParamValueLabel : public ParameterValueLabel
{
public:
  using ParameterValueLabel::ParameterValueLabel;
  std::shared_ptr<Font> getFont () const override;
  int getFontHeight () const override;
};