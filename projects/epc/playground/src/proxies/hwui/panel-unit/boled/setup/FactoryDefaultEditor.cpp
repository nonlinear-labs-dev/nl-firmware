#include "FactoryDefaultEditor.h"
#include <Application.h>
#include <glibmm/ustring.h>
#include <http/UndoScope.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/controls/Point.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <serialization/PresetManagerSerializer.h>
#include <presets/EditBuffer.h>
#include <thread>
#include "use-cases/PresetManagerUseCases.h"
#include <use-cases/SettingsUseCases.h>

static const Rect c_fullRightSidePosition(129, 16, 126, 48);

FactoryDefaultEditor::FactoryDefaultEditor()
    : ControlWithChildren(Rect(0, 0, 0, 0))
{
  addLabel("Attention! This action is not undoable!");
  addControl<Button>("OK", Button::getButtonPos(Buttons::BUTTON_C).getMovedBy(Point(-128, -16)));
  addControl<Button>("Cancel", Button::getButtonPos(Buttons::BUTTON_D).getMovedBy(Point(-128, -16)));
}

void FactoryDefaultEditor::addLabel(const Glib::ustring &text)
{
  addControl<MultiLineLabel>(text)->setPosition(Rect(3, 0, 119, 24));
}

void FactoryDefaultEditor::setPosition(const Rect &)
{
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