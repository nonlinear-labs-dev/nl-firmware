#pragma once

#include "LinearScaleConverter.h"

class Fine12STScaleConverter : public LinearScaleConverter
{
 private:
  typedef LinearScaleConverter super;

 public:
  Fine12STScaleConverter();
  ~Fine12STScaleConverter() override;
};

class Fine24STScaleConverter : public LinearScaleConverter
{
 private:
  typedef LinearScaleConverter super;

 public:
  Fine24STScaleConverter();
};
