#pragma once
#include "ModulateableParameterLayouts.h"
#include "UnmodulatebaleParameterLayouts.h"

class ModulateableUnisonParameterLayout : public ModulateableParameterSelectLayout2
{
 public:
  using ModulateableParameterSelectLayout2::ModulateableParameterSelectLayout2;

 protected:
  Carousel *createCarousel(const Rect &rect) override;
};

class UnmodulateableUnisonParameterLayout : public UnmodulateableParameterSelectLayout2
{
  using UnmodulateableParameterSelectLayout2::UnmodulateableParameterSelectLayout2;

 protected:
  Carousel *createCarousel(const Rect &rect) override;
};
