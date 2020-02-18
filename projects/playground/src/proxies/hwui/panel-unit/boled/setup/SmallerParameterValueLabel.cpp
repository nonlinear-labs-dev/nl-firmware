#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <parameters/Parameter.h>
#include "SmallerParameterValueLabel.h"

std::shared_ptr<Font> SmallerParamValueLabel::getFont() const
{
  return Oleds::get().getFont("Emphase-8-Regular", getFontHeight());
}

int SmallerParamValueLabel::getFontHeight() const
{
  return 8;
}
