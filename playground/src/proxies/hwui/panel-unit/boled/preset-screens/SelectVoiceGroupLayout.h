#pragma once
#include <proxies/hwui/DFBLayout.h>
#include "PresetManagerLayout.h"

class SelectVoiceGroupLayout : public DFBLayout
{
public:
  SelectVoiceGroupLayout(PresetManagerLayout *parent);

  bool onButton(Buttons i, bool down, ::ButtonModifiers modifiers) override;
private:
  PresetManagerLayout* m_parent;
};
