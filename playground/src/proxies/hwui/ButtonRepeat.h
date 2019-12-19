#pragma once

#include "playground.h"
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <functional>

class ButtonRepeat : public sigc::trackable
{
 public:
  ButtonRepeat(std::function<void()> cb);
  ~ButtonRepeat();

 private:
  bool onButtonRepeatInitialTimeoutElapsed(std::function<void()> cb);
  bool onButtonRepeatRegularTimeoutElapsed(std::function<void()> cb);

  sigc::connection m_buttonRepeat;
};
