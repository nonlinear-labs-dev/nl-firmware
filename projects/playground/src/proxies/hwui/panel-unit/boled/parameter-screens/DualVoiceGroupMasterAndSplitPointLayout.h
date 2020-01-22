#pragma once

#include <proxies/hwui/buttons.h>
#include "UnmodulatebaleParameterLayouts.h"
#include "ModulateableParameterLayouts.h"

class DualVoiceGroupMasterAndSplitPointLayout : public ModulateableParameterSelectLayout2
{
 public:
  DualVoiceGroupMasterAndSplitPointLayout();
  ~DualVoiceGroupMasterAndSplitPointLayout();
 protected:
  Carousel *createCarousel(const Rect &rect) override;

  ModuleCaption *createModuleCaption() const override;

  sigc::connection m_connectionEditBuffer;
  sigc::connection m_connectionVoiceGroup;
};
