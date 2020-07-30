#pragma once
#include "SpecialParameterCarousel.h"

class MonoParameterCarousel : public SpecialParameterCarousel
{
 public:
  using SpecialParameterCarousel::SpecialParameterCarousel;

 private:
  std::vector<ParameterId> getParameterIdsForMode(SoundType type) override;
};
