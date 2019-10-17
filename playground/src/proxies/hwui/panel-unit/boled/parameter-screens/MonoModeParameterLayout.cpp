#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MonoParameterCarousel.h>
#include "MonoModeParameterLayout.h"

Parameter *MonoModeParameterLayout::getCurrentParameter() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(eb->getType() == SoundType::Split)
    return ParameterLayout2::getCurrentParameter();
  else
    return eb->getSelected(VoiceGroup::I);
}

MonoModeParameterLayout::MonoModeParameterLayout()
    : UnmodulateableParameterSelectLayout2()
{
}

Carousel *MonoModeParameterLayout::createCarousel(const Rect &rect)
{
  return new MonoParameterCarousel(rect);
}
