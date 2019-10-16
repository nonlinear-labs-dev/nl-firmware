#pragma once

#include <proxies/hwui/buttons.h>
#include "UnmodulatebaleParameterLayouts.h"

class DualSpecialParameterScreen : public UnmodulateableParameterSelectLayout2
{
 public:
  DualSpecialParameterScreen();
  ~DualSpecialParameterScreen();
 protected:
  Parameter *getCurrentParameter() const override;
  Carousel *createCarousel(const Rect &rect) override;

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

  Parameter *getCurrentEditParameter() const override;

  ModuleCaption *createModuleCaption() const override;

  sigc::connection m_connection;
};
