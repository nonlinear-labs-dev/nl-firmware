#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupMasterParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/DualSpecialParameterModuleCaption.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include "DualVoiceGroupMasterAndSplitPointLayout.h"

Parameter *DualVoiceGroupMasterAndSplitPointLayout::getCurrentParameter() const
{
  if(auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected())
    if(auto sp = dynamic_cast<SplitPointParameter *>(selected))
      return sp;

  return ParameterLayout2::getCurrentParameter();
}

DualVoiceGroupMasterAndSplitPointLayout::DualVoiceGroupMasterAndSplitPointLayout()
    : UnmodulateableParameterSelectLayout2()
{
  m_connection = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &DualVoiceGroupMasterAndSplitPointLayout::update)));

  m_connection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &DualVoiceGroupMasterAndSplitPointLayout::update));
}

void DualVoiceGroupMasterAndSplitPointLayout::update()
{
  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected();
  getCarousel()->setup(selected);

  for(auto &i : getControls<DualSpecialParameterModuleCaption>())
  {
    i->updateText(selected);
  }

  if(auto valueLabel = findControlOfType<ParameterValueLabel>())
    valueLabel->updateParameter(selected);

  if(auto nameLabel = findControlOfType<ParameterNameLabel>())
    nameLabel->updateParameter(selected);

  for(auto &i : getControls<Button>())
  {
    if(i->getPosition().getPosition().getX() == Button::getButtonPos(Buttons::BUTTON_A).getPosition().getX())
    {
      auto text = to_string("Select ") + (vg == VoiceGroup::I ? "II" : "I");
      i->setText({ text, 0 });
    }
  }
}

DualVoiceGroupMasterAndSplitPointLayout::~DualVoiceGroupMasterAndSplitPointLayout()
{
  m_connection.disconnect();
}

Carousel *DualVoiceGroupMasterAndSplitPointLayout::createCarousel(const Rect &rect)
{
  return new VoiceGroupMasterParameterCarousel(rect);
}

ModuleCaption *DualVoiceGroupMasterAndSplitPointLayout::createModuleCaption() const
{
  return new DualSpecialParameterModuleCaption(Rect(0, 0, 64, 13));
}

Parameter *DualVoiceGroupMasterAndSplitPointLayout::getCurrentEditParameter() const
{
  return getCurrentParameter();
}

bool DualVoiceGroupMasterAndSplitPointLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_A)
  {
    Application::get().getHWUI()->toggleCurrentVoiceGroup();
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

  if(down && i == Buttons::BUTTON_EDIT)
  {
    Application::get().getHWUI()->setFocusAndMode({UIFocus::Parameters, UIMode::Edit, UIDetail::Init});
    return true;
  }

  return ParameterSelectLayout2::onButton(i, down, modifiers);
}
