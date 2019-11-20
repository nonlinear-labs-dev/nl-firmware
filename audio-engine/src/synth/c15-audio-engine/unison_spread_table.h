#pragma once

/******************************************************************************/
/** @file       unison_spread_table.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      provide unison spread values (up to total voices)
    @note       wrapped in template now
    @todo
*******************************************************************************/

#include <stdint.h>

template <uint32_t Size> struct UnisonSpreadTable
{
  float m_detune[Size][Size] = {}, m_phase[Size][Size] = {}, m_pan[Size][Size] = {};
  inline UnisonSpreadTable()
  {
    int32_t u_voice, u_index;
    for(uint32_t i = 0; i < Size; i++)
    {
      // prepare for unison voice
      u_voice = static_cast<int32_t>(i) + 1;
      // unison detune
      u_index = -(static_cast<int32_t>(i) >> 1);
      for(uint32_t p = 0; p <= i; p++)
      {
        m_detune[i][p] = static_cast<float>(u_index);
        u_index += 1;
      }
      // unison phase and pan
      u_index = -static_cast<int32_t>(i);
      for(uint32_t p = 0; p <= i; p++)
      {
        m_phase[i][p] = static_cast<float>(u_index) / static_cast<float>(2 * u_voice);
        m_pan[i][p] = static_cast<float>(u_index) / static_cast<float>(i < 1 ? 1 : i);
        u_index += 2;
      }
      // zero padding
      for(uint32_t p = i + 1; p < Size; p++)
      {
        m_detune[i][p] = 0.f;
        m_phase[i][p] = 0.f;
        m_pan[i][p] = 0.f;
      }
    }
  }
};
