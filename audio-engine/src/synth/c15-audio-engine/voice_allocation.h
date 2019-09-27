#pragma once

/******************************************************************************/
/** @file       voice_allocation.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      based on former LPC implementation (nl_tcd_valloc.c)
                (note: the allocation returns the allocated voice)
    @todo
*******************************************************************************/

#include "c15_config.h"

template<uint32_t Size>
class VoiceAllocation
{
public:
    inline VoiceAllocation()
    {}
    inline void init(const uint32_t _assignable)
    {
        m_assignable = _assignable;
        for(uint32_t i = 0; i < m_assignable; i++)
        {
            m_voice_state[i] = -1;
        }
        m_num_assigned = m_oldest_assigned = m_youngest_assigned = m_oldest_released = 0;
        m_youngest_released = m_assignable - 1;
        for(uint32_t i = 0; i < m_assignable - 1; i++)
        {
            m_next_released[i] = i + 1;
        }
    }
    inline uint32_t keyDown(const uint32_t _keyPos)
    {
        uint32_t v;
        if(m_num_assigned < m_assignable)
        {
            if(m_num_assigned == 0)
            {
                m_oldest_assigned = m_youngest_assigned = m_oldest_released;
            }
            v = m_oldest_released;
            m_oldest_released = m_next_released[v];
            m_num_assigned++;
        }
        else
        {
            v = m_oldest_assigned;
            m_oldest_assigned = m_next_assigned[v];
        }
        m_next_assigned[m_youngest_assigned] = v;
        m_previous_assigned[v] = m_youngest_assigned;
        m_youngest_assigned = v;
        m_voice_state[v] = static_cast<int32_t>(_keyPos);
        return v;
    }
    inline uint32_t keyUp(const uint32_t _keyPos)
    {
        uint32_t v;
        for(v = 0; v < m_assignable; v++)
        {
            if(m_voice_state[v] == static_cast<int32_t>(_keyPos))
            {
                m_next_released[m_youngest_released] = v;
                m_youngest_released = v;
                if(m_num_assigned == m_assignable)
                {
                    m_oldest_released = v;
                }
                m_num_assigned--;
                if(m_oldest_assigned == v)
                {
                    m_oldest_assigned = m_next_assigned[v];
                }
                else if(m_youngest_assigned == v)
                {
                    m_youngest_assigned = m_previous_assigned[v];
                }
                else
                {
                    m_next_assigned[m_previous_assigned[v]] = m_next_assigned[v];
                    m_previous_assigned[m_next_assigned[v]] = m_previous_assigned[v];
                }
                m_voice_state[v] = -1;
                break;
            }
        }
        return v;
    }
private:
    uint32_t m_previous_assigned[Size], m_next_assigned[Size], m_next_released[Size],
    m_assignable, m_num_assigned, m_oldest_assigned, m_youngest_assigned,
    m_oldest_released, m_youngest_released = {};
    int32_t m_voice_state[Size] = {};
};

using GlobalVoiceAllocation = VoiceAllocation<C15::Config::total_polyphony>;
using LocalVoiceAllocation = VoiceAllocation<C15::Config::local_polyphony>;
