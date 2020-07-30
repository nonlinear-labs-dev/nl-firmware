#include <Application.h>
#include <io/ToPlaycontrollerBridge.h>
#include "Ribbon.h"
#include <io/Bridges.h>
#include <io/FromPlaycontrollerBridge.h>

Ribbon::Ribbon()
    : m_upperRibbon(true)
{
  set_draw_value(false);
  set_digits(0);
  set_range(0, 16000);
}

bool Ribbon::isUpperRibbon()
{
  return true;
}

void Ribbon::on_value_changed()
{
  auto b = Application::get().getBridges()->getBridge<FromPlaycontrollerBridge>();
  b->sendRibbonPosition(isUpperRibbon(), get_value());
}
