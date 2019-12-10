#pragma once

#include <proxies/hwui/buttons.h>
#include "UnmodulatebaleParameterLayouts.h"

class DualVoiceGroupMasterAndSplitPointLayout : public UnmodulateableParameterSelectLayout2
{
 public:
  DualVoiceGroupMasterAndSplitPointLayout();
  ~DualVoiceGroupMasterAndSplitPointLayout();
 protected:
  Carousel *createCarousel(const Rect &rect) override;

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

  ModuleCaption *createModuleCaption() const override;

  sigc::connection m_connectionEditBuffer;
  sigc::connection m_connectionVoiceGroup;
};
