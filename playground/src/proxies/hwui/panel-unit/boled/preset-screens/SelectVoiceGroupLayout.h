#pragma once
#include <proxies/hwui/DFBLayout.h>

class SelectVoiceGroupLayout : public DFBLayout
{
public:
  SelectVoiceGroupLayout();

  bool onButton(Buttons i, bool down, ::ButtonModifiers modifiers) override;
};
