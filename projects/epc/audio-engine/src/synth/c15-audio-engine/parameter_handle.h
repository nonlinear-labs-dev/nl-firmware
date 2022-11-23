#pragma once

/******************************************************************************/
/** @file       parameter_handle.h
    @date
    @version    1.7-3
    @author     M. Seeber
    @brief      new main parameter handle
    @todo
*******************************************************************************/

#include "parameter_storage.h"

// new Handle

namespace Engine
{

  namespace Handle
  {

    struct Clock_Handle
    {
      uint32_t m_index = 0, m_div_fast = 0, m_div_slow = 0;
      bool m_fast = false, m_slow = false;
      inline void init(const uint32_t _upsampleIndex)
      {
        const uint32_t samplerate = C15::Config::clock_rates[_upsampleIndex][0];
        m_div_fast = samplerate / C15::Config::clock_rates[_upsampleIndex][1];
        m_div_slow = samplerate / C15::Config::clock_rates[_upsampleIndex][2];
      }
      inline void render()
      {
        m_fast = (m_index % m_div_fast) == 0;
        m_index = m_index % m_div_slow;
        m_slow = m_index == 0;
        m_index++;
      }
    };

    struct ParameterHandle
    {
      Storage::LayerParameterStorage m_layer[C15::Properties::num_of_VoiceGroups] = {};
      Storage::GlobalParameterStorage m_global = {};
      inline void init()
      {
        m_global.m_parameters.m_assignment.reset();
        for(uint32_t vg = 0; vg < C15::Properties::num_of_VoiceGroups; vg++)
        {
          m_layer[vg].m_assignment.reset();  // resets local modulateables
          m_layer[vg].m_polyphonic.m_assignment.reset();
          m_layer[vg].m_monophonic.m_assignment.reset();
        }
        m_global.init();
      }
    };

  }  // namespace Engine::Handle

}  // namespace Engine
