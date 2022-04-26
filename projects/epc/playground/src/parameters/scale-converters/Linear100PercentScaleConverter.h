#pragma once

#include "LinearScaleConverter.h"

class Linear100PercentScaleConverter : public LinearScaleConverter
{
 public:
  Linear100PercentScaleConverter();
  ~Linear100PercentScaleConverter() override;
};

class Linear200PercentScaleConverter : public LinearScaleConverter
{
 public:
  Linear200PercentScaleConverter();
  ~Linear200PercentScaleConverter() override;
};
