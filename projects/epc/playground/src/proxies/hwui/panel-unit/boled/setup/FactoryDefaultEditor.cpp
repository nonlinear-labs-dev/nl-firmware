#include "FactoryDefaultEditor.h"
#include <Application.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <use-cases/SettingsUseCases.h>

FactoryDefaultEditor::FactoryDefaultEditor()
    : ControlWithChildren(Rect(0, 0, 0, 0))
{
  addLabel("Attention! This action is not undoable!");
  addControl<Button>("OK", Button::getButtonPos(Buttons::BUTTON_C).getMovedBy({-128, -16}));
  addControl<Button>("Cancel", Button::getButtonPos(Buttons::BUTTON_D).getMovedBy({-128, -16}));
}

void FactoryDefaultEditor::addLabel(const Glib::ustring &text)
{
  addControl<MultiLineLabel>(text)->setPosition({3, 0, 119, 24});
}

void FactoryDefaultEditor::setPosition(const Rect &)
{
  const auto static c_fullRightSidePosition = Rect{129, 16, 126, 48};
  ControlWithChildren::setPosition(c_fullRightSidePosition);
}

bool FactoryDefaultEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    if(i == Buttons::BUTTON_C)
    {
      SettingsUseCases useCases(*Application::get().getSettings());
      useCases.factoryDefault();
      return true;
    }
    else if(i == Buttons::BUTTON_D)
    {
      diveUp();
      return true;
    }
  }
  return false;
}