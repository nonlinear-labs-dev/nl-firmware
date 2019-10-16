#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualParameterScreenCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include "DualSpecialParameterScreen.h"

Parameter *DualSpecialParameterScreen::getCurrentParameter() const
{
  if(auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(VoiceGroup::I))
    if(auto sp = dynamic_cast<SplitPointParameter *>(selected))
      return sp;

  return ParameterLayout2::getCurrentParameter();
}

DualSpecialParameterScreen::DualSpecialParameterScreen()
    : UnmodulateableParameterSelectLayout2()
{
  m_connection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged([this]() {
    auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
    getCarousel()->setup(Application::get().getPresetManager()->getEditBuffer()->getSelected(vg));
  });
}

DualSpecialParameterScreen::~DualSpecialParameterScreen()
{
  m_connection.disconnect();
}

Carousel *DualSpecialParameterScreen::createCarousel(const Rect &rect)
{
  return new DualParameterScreenCarousel(rect);
}

ModuleCaption *DualSpecialParameterScreen::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

Parameter *DualSpecialParameterScreen::getCurrentEditParameter() const
{
  return getCurrentParameter();
}

bool DualSpecialParameterScreen::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_A)
  {
    toggleVoiceGroup();
    return true;
  }

  if(down && i == Buttons::BUTTON_C)
  {
    openLoadPresetIntoVoiceGroup();
    return true;
  }

  return ParameterSelectLayout2::onButton(i, down, modifiers);
}

void DualSpecialParameterScreen::toggleVoiceGroup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto currentVG = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  if(currentVG == VoiceGroup::I && dynamic_cast<const SplitPointParameter *>(eb->getSelected(currentVG)))
  {
    getCarousel()->turn();  //select next parameter that is not Split Point!
  }

  Application::get().getVoiceGroupSelectionHardwareUI()->toggleHWEditBufferSelection();
}

void DualSpecialParameterScreen::openLoadPresetIntoVoiceGroup()
{
}