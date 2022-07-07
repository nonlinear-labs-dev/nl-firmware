#pragma once

#include "LinearScaleConverter.h"

class LinearBipolar60DbScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar60DbScaleConverter();
  ~LinearBipolar60DbScaleConverter() override;
};

class LinearBipolar60DbTScaleConverter : public LinearScaleConverter
{
 public:
  LinearBipolar60DbTScaleConverter();
  ~LinearBipolar60DbTScaleConverter() override = default;
};