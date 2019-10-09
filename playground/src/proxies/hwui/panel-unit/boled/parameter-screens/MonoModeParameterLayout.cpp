#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MonoParameterCarousel.h>
#include "MonoModeParameterLayout.h"

Parameter *MonoModeParameterLayout::getCurrentParameter() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  switch(eb->getType())
  {
    case SoundType::Single:
    case SoundType::Split:
      return ParameterLayout2::getCurrentParameter();
    case SoundType::Layer:
      return eb->getSelected(VoiceGroup::I);
    case SoundType::Invalid:
      nltools::fail("", __FILE__, __LINE__, __FUNCTION__);
  }

  return nullptr;
}

MonoModeParameterLayout::MonoModeParameterLayout()
    : UnmodulateableParameterSelectLayout2{}
{
}

Carousel *MonoModeParameterLayout::createCarousel(const Rect &rect)
{
  return new MonoParameterCarousel(rect);
}

ModuleCaption *MonoModeParameterLayout::createModuleCaption() const
{
  return new MonoModuleCaption(Rect{ 0, 0, 64, 13 });
}
