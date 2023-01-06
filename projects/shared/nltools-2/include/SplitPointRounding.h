#pragma once

#include <parameter_list.h>
#include <cmath>
#include <algorithm>

namespace SplitPointRounding
{

  static constexpr float range = (float) C15::ParameterList[C15::PID::Split_Split_Point].m_pg.m_coarse_cp;

  inline signed round(const float &_value)
  {
    return (signed) std::ceil(range * _value);
  }

  inline float quantize(const float &_value, const unsigned &_steps)
  {
    if(_steps == 0)
      return _value;
    return std::clamp(std::floor(_value * (float) (1 + _steps)) / (float) _steps, 0.0f, 1.0f);
  }

  inline float getModulation(const float &_mod, const float &_modAmount)
  {
    return (float) _modAmount * quantize(_mod, round(std::abs(_modAmount)));
  }

}
