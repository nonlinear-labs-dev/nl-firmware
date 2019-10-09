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

template<uint32_t Voices, uint32_t Keys>
class VoiceAllocator
{
public:
    inline VoiceAllocator()
    {}
    inline void init(const uint32_t _assignable)
    {
        m_assignable = _assignable;
        m_num_assigned = m_oldest_assigned = m_youngest_assigned = m_oldest_released = 0;
        m_youngest_released = m_assignable - 1;
        for(uint32_t i = 0; i < m_assignable - 1; i++)
        {
            m_next_released[i] = i + 1;
        }
        for(uint32_t i = 0; i < Keys; i++)
        {
            m_key_state[i] = -1;
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
        m_key_state[_keyPos] = v;
        return v;
    }
    inline uint32_t keyUp(const uint32_t _keyPos)
    {
        const int32_t v = m_key_state[_keyPos];
        if(v != -1)
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
            m_key_state[_keyPos] = -1;
        }
        return v;
    }
private:
    uint32_t m_previous_assigned[Voices], m_next_assigned[Voices], m_next_released[Voices],
    m_assignable, m_num_assigned, m_oldest_assigned, m_youngest_assigned,
    m_oldest_released, m_youngest_released = {};
    int32_t m_key_state[Keys] = {};
};

struct PolyKeyEvent
{
    uint32_t m_voiceId, m_unisonIndex = 0;
    bool m_stolen = false;
    inline void update(const uint32_t _voiceId, const uint32_t _unisonIndex, const bool _stolen)
    {
        m_voiceId = _voiceId;
        m_unisonIndex = _unisonIndex;
        m_stolen = _stolen;
    }
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
    inline void add(const uint32_t _voiceId, const uint32_t _unisonIndex, const bool _stolen)
    {
        m_data[m_length].update(_voiceId, _unisonIndex, _stolen);
        m_length++;
    }
private:
    PolyKeyEvent m_data[Voices];
    uint32_t m_index, m_length = 0;
};

template<uint32_t Voices, uint32_t Keys>
class VoiceAllocation
{
public:
    PolyKeyPacket<Voices> m_traversal;
    inline VoiceAllocation()
    {}
    inline void init(const uint32_t _voices)
    {
        m_unison = 1;
        m_voices = _voices;
        m_alloc.init(_voices);
        reset();
    }
    inline void keyDown(const uint32_t _keyPos)
    {
        const uint32_t voiceOffset = m_unison * m_alloc.keyDown(_keyPos);
        uint32_t voiceId;
        m_traversal.init();
        for(uint32_t unisonIndex = 0; unisonIndex < m_unison; unisonIndex++)
        {
            voiceId = voiceOffset + unisonIndex;
            m_traversal.add(voiceId, unisonIndex, m_active[voiceId]);
            m_active[voiceId] = true;
        }
    }
    inline void keyUp(const uint32_t _keyPos)
    {
        const int32_t voice = m_alloc.keyUp(_keyPos);
        m_traversal.init();
        if(voice != -1)
        {
            const uint32_t voiceOffset = m_unison * voice;
            uint32_t voiceId;
            for(uint32_t unisonIndex = 0; unisonIndex < m_unison; unisonIndex++)
            {
                voiceId = voiceOffset + unisonIndex;
                m_traversal.add(voiceId, unisonIndex, false);
                m_active[voiceId] = false;
            }
        }
    }
    inline void setUnison(const uint32_t _voices)
    {
        m_unison = _voices;
        m_alloc.init(m_voices / _voices);
    }
    inline void reset()
    {
        m_traversal.init();
        for(uint32_t i = 0; i < m_voices; i++)
        {
            m_active[i] = false;
        }
    }
private:
    VoiceAllocator<Voices, Keys> m_alloc;
    bool m_active[Voices] = {};
    uint32_t m_voices, m_unison = 0;
};

struct DualVoiceAllocation
{
    VoiceAllocation<C15::Config::total_polyphony, C15::Config::key_count> m_global;
    VoiceAllocation<C15::Config::local_polyphony, C15::Config::key_count> m_local[2];
    inline void init()
    {
        m_global.init(C15::Config::total_polyphony);
        m_local[0].init(C15::Config::local_polyphony);
        m_local[1].init(C15::Config::local_polyphony);
    }
};
