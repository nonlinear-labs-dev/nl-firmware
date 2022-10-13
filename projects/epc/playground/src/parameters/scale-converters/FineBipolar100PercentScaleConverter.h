#pragma once

#include "LinearScaleConverter.h"

class FineBipolar100PercentScaleConverter : public LinearScaleConverter
{
public:
    FineBipolar100PercentScaleConverter();
    ~FineBipolar100PercentScaleConverter() override;
};
