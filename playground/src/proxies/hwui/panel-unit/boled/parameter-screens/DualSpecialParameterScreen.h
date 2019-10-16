#pragma once
#include "UnmodulatebaleParameterLayouts.h"

class DualSpecialParameterScreen : public UnmodulateableParameterSelectLayout2
{
 public:
  DualSpecialParameterScreen();
 protected:
  Parameter *getCurrentParameter() const override;
  Carousel *createCarousel(const Rect &rect) override;

  ModuleCaption *createModuleCaption() const override;
};
