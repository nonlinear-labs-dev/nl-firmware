#pragma once

#include "LED.h"

class Application;

class TwoStateLED: public LED
{
  public:
    TwoStateLED(int id);
    virtual ~TwoStateLED();

    enum LedState
    {
      OFF,
      ON,
      BLINK
    };

    void setState(LedState state);
    LedState getState();
    void init ();

  private:
    void switchLED (bool onOrOff);
    void onBlinkUpdate (int blinkCount);

    LedState m_state;
    sigc::connection m_blinkTimer;
};
