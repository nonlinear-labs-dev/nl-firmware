#pragma once
#include "proxies/hwui/Layout.h"

class DoYouWantToStopRecorderLayout : public Layout
{
 public:
  DoYouWantToStopRecorderLayout();
  bool onButton(Buttons i, bool down, ::ButtonModifiers modifiers) override;
};
