#pragma once

#include "LED.h"

class FourStateLED : public LED
{
 public:
  FourStateLED();

  void setState(char state);
  char getState() const;
  void syncBBBB();

 private:
  char m_state;
};
