#pragma once

#include "UnmodulatebaleParameterLayouts.h"

class MonoModeParameterLayout : public UnmodulateableParameterSelectLayout2
{
 public:
  MonoModeParameterLayout();

 protected:
  Parameter *getCurrentParameter() const override;

  Carousel *createCarousel(const Rect &rect) override;
};
