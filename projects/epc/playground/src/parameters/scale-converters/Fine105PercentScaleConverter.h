#pragma once

#include "LinearScaleConverter.h"

class Fine105PercentScaleConverter : public LinearScaleConverter
{
 public:
  Fine105PercentScaleConverter();
  ~Fine105PercentScaleConverter() override;
};

class Fine200PercentScaleConverter : public LinearScaleConverter
{
 public:
  Fine200PercentScaleConverter();
  ~Fine200PercentScaleConverter() override;
};
