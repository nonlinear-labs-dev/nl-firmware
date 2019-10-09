#pragma once

/******************************************************************************/
/** @file       key_event.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      primitives: key event data structure (index, active, steal, unison)
                (keyPos and velocity are not stored here)
    @todo
*******************************************************************************/

#include <stdint.h>

struct PolyKeyEvent
{
    uint32_t m_localIndex = 0,
             m_voiceId = 0,
             m_unisonIndex = 0;
    bool m_active = false,
         m_stolen = false;
};

template<uint32_t Voices>
class PolyKeyPacket
{
public:
    inline PolyKeyPacket()
    {}
    inline void init()
    {
        m_length = 0;
    }
    inline PolyKeyEvent* first()
    {
        reset();
        return &m_data[m_index];
    }
    inline bool running()
    {
        return m_index < m_length;
    }
    inline PolyKeyEvent* next()
    {
        m_index++;
        return &m_data[m_index];
    }
    inline void reset()
    {
        m_index = 0;
    }
    inline void add(const uint32_t _localIndex, const uint32_t _voiceId, const uint32_t _unisonIndex, const bool _active, const bool _stolen)
    {
        m_data[m_length].m_localIndex = _localIndex;
        m_data[m_length].m_voiceId = _voiceId;
        m_data[m_length].m_unisonIndex = _unisonIndex;
        m_data[m_length].m_active = _active;
        m_data[m_length].m_stolen = _stolen;
        m_length++;
    }
private:
    PolyKeyEvent m_data[Voices];
    uint32_t m_index = 0,
             m_length = 0;
};
