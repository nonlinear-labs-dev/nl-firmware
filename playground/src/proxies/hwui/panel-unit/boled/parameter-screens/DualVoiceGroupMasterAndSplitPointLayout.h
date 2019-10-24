#pragma once

#include <proxies/hwui/buttons.h>
#include "UnmodulatebaleParameterLayouts.h"

class DualVoiceGroupMasterAndSplitPointLayout : public UnmodulateableParameterSelectLayout2
{
 public:
  DualVoiceGroupMasterAndSplitPointLayout();
  ~DualVoiceGroupMasterAndSplitPointLayout();
 protected:
  Parameter *getCurrentParameter() const override;
  Carousel *createCarousel(const Rect &rect) override;

  void update();

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

  Parameter *getCurrentEditParameter() const override;

  ModuleCaption *createModuleCaption() const override;

  sigc::connection m_connection;
};
