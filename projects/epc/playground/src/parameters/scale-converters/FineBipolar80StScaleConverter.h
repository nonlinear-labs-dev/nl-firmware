#pragma once

#include "LinearScaleConverter.h"

class FineBipolar80StScaleConverter : public LinearScaleConverter
{
 public:
  FineBipolar80StScaleConverter();
  ~FineBipolar80StScaleConverter() override;
};

class FineBipolar160StScaleConverter : public LinearScaleConverter
{
 public:
  FineBipolar160StScaleConverter();
  ~FineBipolar160StScaleConverter() = default;
};
