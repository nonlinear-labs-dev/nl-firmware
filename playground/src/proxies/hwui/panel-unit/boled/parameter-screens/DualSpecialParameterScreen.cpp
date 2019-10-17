#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupMasterParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include "DualSpecialParameterScreen.h"

Parameter *DualSpecialParameterScreen::getCurrentParameter() const
{
  if(auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected())
    if(auto sp = dynamic_cast<SplitPointParameter *>(selected))
      return sp;

  return ParameterLayout2::getCurrentParameter();
}

DualSpecialParameterScreen::DualSpecialParameterScreen()
    : UnmodulateableParameterSelectLayout2()
{
  m_connection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged([this]() {
    auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
    auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected();
    getCarousel()->setup(selected);
    for(auto &i : getControls<DualSpecialParameterModuleCaption>())
    {
      i->updateText(selected);
    }
    for(auto &i : getControls<Button>())
    {
      if(i->getPosition().getPosition().getX() == Button::getButtonPos(Buttons::BUTTON_A).getPosition().getX())
      {
        auto text = to_string("Select ") + (vg == VoiceGroup::I ? "II" : "I");
        i->setText({ text, 0 });
      }
    }
  });
}

DualSpecialParameterScreen::~DualSpecialParameterScreen()
{
  m_connection.disconnect();
}

Carousel *DualSpecialParameterScreen::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
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
    Application::get().getVoiceGroupSelectionHardwareUI()->toggleHWEditBufferSelection();
    return true;
  }

  if(down && i == Buttons::BUTTON_C)
  {
    return true;
  }

  if(down && i == Buttons::BUTTON_D)
  {
    if(modifiers[ButtonModifier::SHIFT])
      getCarousel()->antiTurn();
    else
      getCarousel()->turn();
    return true;
  }

  return ParameterSelectLayout2::onButton(i, down, modifiers);
}