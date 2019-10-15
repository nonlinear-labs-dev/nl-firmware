#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualParameterScreenCarousel.h>
#include "DualParameterScreenLayout.h"

Parameter *DualParameterScreenLayout::getCurrentParameter() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(eb->getType() == SoundType::Split)
    return ParameterLayout2::getCurrentParameter();
  else
    return eb->getSelected(VoiceGroup::I);
}

DualParameterScreenLayout::DualParameterScreenLayout()
    : UnmodulateableParameterSelectLayout2()
{
}

Carousel *DualParameterScreenLayout::createCarousel(const Rect &rect)
{
  return new DualParameterScreenCarousel(rect);
}