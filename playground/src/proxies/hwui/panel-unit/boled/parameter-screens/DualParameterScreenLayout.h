#pragma once
#include "UnmodulatebaleParameterLayouts.h"

class DualParameterScreenLayout : public UnmodulateableParameterSelectLayout2
{
 public:
  DualParameterScreenLayout();
 protected:
  Parameter *getCurrentParameter() const override;
  Carousel *createCarousel(const Rect &rect) override;
};
