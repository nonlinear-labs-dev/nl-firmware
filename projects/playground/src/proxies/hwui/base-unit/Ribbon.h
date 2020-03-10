#pragma once

#include "proxies/hwui/FourStateLED.h"

#define NUM_LEDS_PER_RIBBON 33

class Application;

class Ribbon
{
 public:
  Ribbon();
  virtual ~Ribbon();

  void syncBBBB();

  [[nodiscard]] const FourStateLED* getLED(int ledPos) const;

 protected:
  void initLEDs();
  void setLEDState(int ledPos, char state);
  void resetLEDs();
  void setLEDsForValueUniPolar(tDisplayValue paramVal);
  void setLEDsUniPolar(int paramValIdx);
  void setLEDsForValueBiPolar(tDisplayValue paramVal);

  virtual int posToLedID(int pos) const = 0;

  void debugTrace();

 private:
  char getLEDStateForBipolarValue(int led, tDisplayValue v) const;
  char handleCenter(tDisplayValue v) const;

  FourStateLED m_leds[NUM_LEDS_PER_RIBBON];
};
