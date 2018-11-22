#pragma once

#include "LED.h"
#include <fstream>

class FourStateLED : public LED
{
 public:
  FourStateLED();
  virtual ~FourStateLED();

  void setState(char state);
  char getState() const;
  void syncBBBB();

 private:
  char m_state;
};
