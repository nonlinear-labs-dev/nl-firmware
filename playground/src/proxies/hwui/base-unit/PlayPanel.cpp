#include <glib.h>
#include "PlayPanel.h"

void PlayPanel::onTimeout()
{
  m_oled.redraw();
}

SOLED &PlayPanel::getSOLED()
{
  return m_oled;
}
