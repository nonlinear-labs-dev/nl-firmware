#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UnisonParameterCarousel.h>
#include "UnisonParameterLayouts.h"

Carousel *UnmodulateableUnisonParameterLayout::createCarousel(const Rect &rect)
{
  return new UnisonParameterCarousel(rect);
}

Carousel *ModulateableUnisonParameterLayout::createCarousel(const Rect &rect)
{
  return new UnisonParameterCarousel(rect);
}
