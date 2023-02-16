#pragma once

#include "soled/SOLED.h"

class Application;

class PlayPanel
{
 public:
  PlayPanel(Oleds& oleds)
      : m_oled(oleds)
  {
  }

  void onTimeout();
  SOLED& getSOLED();

 private:
  SOLED m_oled;
};
