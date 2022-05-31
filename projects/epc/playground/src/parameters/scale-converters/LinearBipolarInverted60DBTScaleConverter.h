#pragma once

#include "LinearScaleConverter.h"

class LinearBipolarInverted60DbtScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolarInverted60DbtScaleConverter();
  ~LinearBipolarInverted60DbtScaleConverter() override;
};
