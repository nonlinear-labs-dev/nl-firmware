#include "UnmodulatebaleParameterLayouts.h"
#include "groups/ScaleGroup.h"
#include "use-cases/EditBufferUseCases.h"
#include "parameter_declarations.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/MCAmountButton.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterBarSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterKnubbelSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h>
#include <proxies/hwui/controls/SwitchVoiceGroupButton.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

UnmodulateableParameterLayout2::UnmodulateableParameterLayout2()
    : super()
{
}

void UnmodulateableParameterLayout2::init()
{
  super::init();
  addButtons();
}

void UnmodulateableParameterLayout2::addButtons()
{
  addControl(new SwitchVoiceGroupButton(Buttons::BUTTON_A));
  addControl(new Button("", Buttons::BUTTON_B));
  addControl(new Button("", Buttons::BUTTON_C));
}

UnmodulateableParameterSelectLayout2::UnmodulateableParameterSelectLayout2()
    : virtual_base()
    , super1()
    , super2()
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  m_signalParameterSelectionChanged = Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &UnmodulateableParameterSelectLayout2::onParameterSelectionChanged), currentVG);
}

void UnmodulateableParameterSelectLayout2::init()
{
  super1::init();
  super2::init();

  if(auto p = getCurrentParameter())
  {
    switch(p->getVisualizationStyle())
    {
      case Parameter::VisualizationStyle::Bar:
      case Parameter::VisualizationStyle::BarFromRight:
        addControl(new SelectedParameterBarSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
      case Parameter::VisualizationStyle::Dot:
        addControl(new SelectedParameterKnubbelSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
    }
  }

  addControl(createParameterValueControl());

  highlight<SelectedParameterBarSlider>();
  highlight<SelectedParameterValue>();
  highlight<ParameterNameLabel>();
}

bool UnmodulateableParameterSelectLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  return ParameterSelectLayout2::onButton(i, down, modifiers);
}

void UnmodulateableParameterSelectLayout2::onParameterSelectionChanged(Parameter *oldP, Parameter *newP)
{
}

UnmodulateableParameterEditLayout2::UnmodulateableParameterEditLayout2()
    : virtual_base()
    , super1()
    , super2()
{
}

void UnmodulateableParameterEditLayout2::init()
{
  super1::init();
  super2::init();

  if(auto p = getCurrentParameter())
  {
    switch(p->getVisualizationStyle())
    {
      case Parameter::VisualizationStyle::Dot:
        addControl(new SelectedParameterKnubbelSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
      case Parameter::VisualizationStyle::Bar:
      case Parameter::VisualizationStyle::BarFromRight:
        addControl(new SelectedParameterBarSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
    }
  }

  addControl(createParameterValueControl());

  highlight<SelectedParameterBarSlider>();
  highlight<SelectedParameterValue>();
  highlight<ParameterNameLabel>();

  if(auto vgButton = findControlOfType<SwitchVoiceGroupButton>())
  {
    remove(vgButton.get());
    addControl(new Button("", Buttons::BUTTON_A));
  }
}

ButtonMenu *UnmodulateableParameterEditLayout2::createMenu(const Rect &rect)
{
  return new ParameterEditButtonMenu(rect);
}

bool UnmodulateableParameterEditLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(super1::onButton(i, down, modifiers))
    return true;

  return super2::onButton(i, down, modifiers);
}
