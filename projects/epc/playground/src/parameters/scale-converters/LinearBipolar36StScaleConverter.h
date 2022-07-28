#pragma once

#include "LinearScaleConverter.h"

class LinearBipolar36StScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar36StScaleConverter();
  ~LinearBipolar36StScaleConverter() override;
};

class LinearBipolar72StScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar72StScaleConverter();
  ~LinearBipolar72StScaleConverter() override = default;
};