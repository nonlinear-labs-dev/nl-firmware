#pragma once
#include "proxies/hwui/Layout.h"

class DoYouWantToStopRecorderPlaybackLayout : public Layout
{
 public:
  DoYouWantToStopRecorderPlaybackLayout();
  bool onButton(Buttons i, bool down, ::ButtonModifiers modifiers) override;
};
