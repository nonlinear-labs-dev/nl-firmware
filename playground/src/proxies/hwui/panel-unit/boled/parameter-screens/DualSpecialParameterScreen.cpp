#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualParameterScreenCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include "DualSpecialParameterScreen.h"

Parameter *DualSpecialParameterScreen::getCurrentParameter() const
{
  return ParameterLayout2::getCurrentParameter();
}

DualSpecialParameterScreen::DualSpecialParameterScreen()
    : UnmodulateableParameterSelectLayout2()
{
}

Carousel *DualSpecialParameterScreen::createCarousel(const Rect &rect)
{
  return new DualParameterScreenCarousel(rect);
}

ModuleCaption *DualSpecialParameterScreen::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}
