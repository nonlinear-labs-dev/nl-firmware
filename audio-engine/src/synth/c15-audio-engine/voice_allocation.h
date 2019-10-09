#pragma once

/******************************************************************************/
/** @file       voice_allocation.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      based on former LPC implementation (nl_tcd_valloc.c)
                - two abstractional layers facilitating different tasks:
                  1) Allocator: basic Voice assignment algorithm
                  2) Allocation: providing global and local allocators, perform
                  Unison, Steal, Split etc.
                - safe against invalid arguments
    @todo
*******************************************************************************/

#include "c15_config.h"
#include "parameter_info.h"
#include "key_event.h"

// Descriptors

using LayerMode = C15::Properties::LayerMode;

enum class AllocatorId
{
    None, Global, Local_I, Local_II, Dual
};

// Primitive Data Structures

struct KeyAssignment
{
    AllocatorId m_origin = AllocatorId::None;
    uint32_t m_voiceId = 0;
    bool m_active = false;
};

struct VoiceAssignment
{
    uint32_t m_keyId = 0;
    bool m_active = false,
         m_stolen = false;
};

// Basic Allocator

template<uint32_t Voices>
class VoiceAllocator
{
public:
    inline VoiceAllocator()
    {
        m_unison = 1;
        init(Voices);
    }
    // keyDown algorithm
    inline uint32_t keyDown()
    {
        uint32_t voiceId;
        if(m_num_assigned < m_assignable)
        {
            if(m_num_assigned == 0)
            {
              m_oldest_assigned = m_youngest_assigned = m_oldest_released;
            }
            voiceId = m_oldest_released;
            m_oldest_released = m_next_released[voiceId];
            m_num_assigned++;
        }
        else
        {
            voiceId = m_oldest_assigned;
            m_oldest_assigned = m_next_assigned[voiceId];
        }
        m_next_assigned[m_youngest_assigned] = voiceId;
        m_previous_assigned[voiceId] = m_youngest_assigned;
        m_youngest_assigned = voiceId;
        return voiceId;
    }
    // keyUp algorithm
    inline uint32_t keyUp(const uint32_t _voiceId)
    {
        m_next_released[m_youngest_released] = _voiceId;
        m_youngest_released = _voiceId;
        if(m_num_assigned == m_assignable)
        {
            m_oldest_released = _voiceId;
        }
        m_num_assigned--;
        if(m_oldest_assigned == _voiceId)
        {
            m_oldest_assigned = m_next_assigned[_voiceId];
        }
        else if(m_youngest_assigned == _voiceId)
        {
            m_youngest_assigned = m_previous_assigned[_voiceId];
        }
        else
        {
            m_next_assigned[m_previous_assigned[_voiceId]] = m_next_assigned[_voiceId];
            m_previous_assigned[m_next_assigned[_voiceId]] = m_previous_assigned[_voiceId];
        }
        return _voiceId;
    }
    // unison setter
    inline void setUnison(const uint32_t _voices)
    {
        m_unison = _voices;
        init(Voices / _voices);
    }
    // (internal) initialization
    inline void init(const uint32_t _assignable)
    {
        m_assignable = _assignable;
        m_num_assigned = m_oldest_assigned = m_youngest_assigned = m_oldest_released = 0;
        m_youngest_released = m_assignable - 1;
        for(uint32_t i = 0; i < m_assignable - 1; i++)
        {
            m_next_released[i] = i + 1;
        }
    }
    // (internal) unison getter
    inline uint32_t getUnison()
    {
        return m_unison;
    }
private:
    uint32_t m_previous_assigned[Voices],
           m_next_assigned[Voices], m_next_released[Voices],
           m_assignable, m_num_assigned, m_oldest_assigned, m_youngest_assigned,
           m_oldest_released, m_youngest_released, m_unison {};
};

// Main Structure

template<uint32_t GlobalVoices, uint32_t LocalVoices, uint32_t Keys>
class VoiceAllocation
{
public:
    PolyKeyPacket<GlobalVoices> m_traversal;
    uint32_t m_unison;
    inline VoiceAllocation()
    {}
    inline void init(LayerMode* _mode)
    {
        m_mode = _mode;
        // prepare lookup tables for local index (I, II) and voices (0 ... LocalVoices -1)
        for(uint32_t v = 0; v < GlobalVoices; v++)
        {
            m_localIndex[v] = v / LocalVoices;
            m_localVoice[v] = v % LocalVoices;
        }
    }
    inline bool keyDown(const uint32_t _keyPos)
    {
        // validation 1 - keyPos_in_range ?
        bool validity = _keyPos < Keys;
        if(validity)
        {
            KeyAssignment* keyState = &m_keyState[_keyPos];
            // validation 2 - key_released ?
            validity = !keyState->m_active;
            if(validity)
            {
                m_traversal.init();
                keyDown_apply(keyState, _keyPos);
                keyDown_confirm(keyState);
            }
        }
        return validity;
    }
    inline bool keyUp(const uint32_t _keyPos)
    {
        // validation 1 - keyPos_in_range ?
        bool validity = _keyPos < Keys;
        if(validity)
        {
            KeyAssignment* keyState = &m_keyState[_keyPos];
            // validation 2 - key_pressed ?
            validity = keyState->m_active;
            if(validity)
            {
                m_traversal.init();
                validity = keyUp_apply(keyState);
            }
            keyUp_confirm(keyState);
        }
        return validity;
    }
    inline bool setSplitPoint(const uint32_t _keyPos)
    {
        // validation - keyPos_in_range ?
        bool validity = _keyPos < Keys;
        if(validity)
        {
            m_splitPoint = _keyPos;
        }
        return validity;
    }
    inline bool setUnison(const AllocatorId _allocator, const float _value)
    {
        uint32_t voices = 1;
        bool validity = false;
        switch(_allocator)
        {
        case AllocatorId::None:
            break;
        case AllocatorId::Global:
            voices += static_cast<uint32_t>(_value * (static_cast<float>(GlobalVoices - 1)));
            clear_keyState(AllocatorId::Global);
            m_global.setUnison(voices);
            m_unison = voices;
            validity = true;
            break;
        case AllocatorId::Local_I:
            voices += static_cast<uint32_t>(_value * (static_cast<float>(LocalVoices - 1)));
            clear_keyState(AllocatorId::Local_I);
            m_local[0].setUnison(voices);
            m_unison = voices;
            validity = true;
            break;
        case AllocatorId::Local_II:
            voices += static_cast<uint32_t>(_value * (static_cast<float>(LocalVoices - 1)));
            clear_keyState(AllocatorId::Local_II);
            m_local[1].setUnison(voices);
            m_unison = voices;
            validity = true;
            break;
        case AllocatorId::Dual:
            break;
        }
        return validity;
    }
    // resets everything
    inline void reset()
    {
        clear_keyState();
    }
private:
    VoiceAllocator<GlobalVoices> m_global;
    VoiceAllocator<LocalVoices> m_local[2];
    KeyAssignment m_keyState[Keys];
    VoiceAssignment m_voiceState[GlobalVoices];
    uint32_t m_localIndex[GlobalVoices],
             m_localVoice[GlobalVoices],
             m_splitPoint = {};
    LayerMode* m_mode;
    inline void keyDown_apply(KeyAssignment* _keyState, const uint32_t _keyPos)
    {
        uint32_t firstVoice, unisonVoices;
        // determine associated allocator (by mode)
        switch(*m_mode)
        {
        case LayerMode::Single:
            _keyState->m_origin = AllocatorId::Global;
            _keyState->m_voiceId = m_global.keyDown();
            unisonVoices = m_global.getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // clear stolen key first (all associated voices will be lost)
            if(m_voiceState[firstVoice].m_active)
            {
                keyUp_confirm(&m_keyState[m_voiceState[firstVoice].m_keyId]);
            }
            // unison loop
            keyDown_unisonLoop(_keyPos, firstVoice, unisonVoices);
            break;
        case LayerMode::Split:
            // determine split target
            if(m_splitPoint > _keyPos)
            {
                _keyState->m_origin = AllocatorId::Local_I;
                _keyState->m_voiceId = m_local[0].keyDown();
                unisonVoices = m_local[0].getUnison();
                firstVoice = _keyState->m_voiceId * unisonVoices;
                // clear stolen key first (all associated voices will be lost)
                if(m_voiceState[firstVoice].m_active)
                {
                    keyUp_confirm(&m_keyState[m_voiceState[firstVoice].m_keyId]);
                }
                // unison loop
                keyDown_unisonLoop(_keyPos, firstVoice, unisonVoices);
            }
            else
            {
                _keyState->m_origin = AllocatorId::Local_II;
                _keyState->m_voiceId = m_local[1].keyDown();
                unisonVoices = m_local[1].getUnison();
                firstVoice = _keyState->m_voiceId * unisonVoices;
                // clear stolen key first (all associated voices will be lost)
                if(m_voiceState[firstVoice].m_active)
                {
                    keyUp_confirm(&m_keyState[m_voiceState[firstVoice].m_keyId]);
                }
                // unison loop
                keyDown_unisonLoop(_keyPos, LocalVoices + firstVoice, unisonVoices);
            }
            break;
        case LayerMode::Layer:
            _keyState->m_origin = AllocatorId::Dual;
            _keyState->m_voiceId = m_local[0].keyDown();
            unisonVoices = m_local[0].getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // clear stolen key first (all associated voices will be lost)
            if(m_voiceState[firstVoice].m_active)
            {
                keyUp_confirm(&m_keyState[m_voiceState[firstVoice].m_keyId]);
            }
            // unison loop
            keyDown_unisonLoop(_keyPos, firstVoice, unisonVoices);
            keyDown_unisonLoop(_keyPos, LocalVoices + firstVoice, unisonVoices);
            break;
        }
    }
    inline bool keyUp_apply(KeyAssignment* _keyState)
    {
        uint32_t firstVoice, unisonVoices;
        // determine associated allocator (by origin)
        switch(_keyState->m_origin)
        {
        case AllocatorId::None:
            return false;
        case AllocatorId::Global:
            unisonVoices = m_global.getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // unison loop
            keyUp_unisonLoop(firstVoice, unisonVoices);
            break;
        case AllocatorId::Local_I:
            unisonVoices = m_local[0].getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // unison loop
            keyUp_unisonLoop(firstVoice, unisonVoices);
            break;
        case AllocatorId::Local_II:
            unisonVoices = m_local[1].getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // unison loop
            keyUp_unisonLoop(LocalVoices + firstVoice, unisonVoices);
            break;
        case AllocatorId::Dual:
            unisonVoices = m_local[0].getUnison();
            firstVoice = _keyState->m_voiceId * unisonVoices;
            // unison loop
            keyUp_unisonLoop(firstVoice, unisonVoices);
            keyUp_unisonLoop(LocalVoices + firstVoice, unisonVoices);
            break;
        }
        return true;
    }
    inline void keyDown_confirm(KeyAssignment* _keyState)
    {
        _keyState->m_active = true;
    }
    inline void keyUp_confirm(KeyAssignment* _keyState)
    {
        _keyState->m_origin = AllocatorId::None;
        _keyState->m_active = false;
    }
    inline void keyDown_unisonLoop(const uint32_t _keyId, const uint32_t _firstVoice, const uint32_t _unisonVoices)
    {
        for(uint32_t unisonIndex = 0; unisonIndex < _unisonVoices; unisonIndex++)
        {
            const uint32_t voiceId = _firstVoice + unisonIndex;
            VoiceAssignment* voiceState = &m_voiceState[voiceId];
            voiceState->m_keyId = _keyId;
            voiceState->m_stolen = voiceState->m_active;
            voiceState->m_active = true;
            m_traversal.add(m_localIndex[voiceId], m_localVoice[voiceId], unisonIndex, voiceState->m_active, voiceState->m_stolen);
        }
    }
    inline void keyUp_unisonLoop(const uint32_t _firstVoice, const uint32_t _unisonVoices)
    {
        for(uint32_t unisonIndex = 0; unisonIndex < _unisonVoices; unisonIndex++)
        {
            const uint32_t voiceId = _firstVoice + unisonIndex;
            VoiceAssignment* voiceState = &m_voiceState[voiceId];
            voiceState->m_stolen = voiceState->m_active = false;
            m_traversal.add(m_localIndex[voiceId], m_localVoice[voiceId], unisonIndex, voiceState->m_active, voiceState->m_stolen);
        }
    }
    inline void clear_keyState()
    {
        m_traversal.init();
        for(uint32_t k = 0; k < Keys; k++)
        {
            KeyAssignment* keyState = &m_keyState[k];
            // find active keys
            if(keyState->m_active)
            {
                keyUp_apply(keyState);
                keyUp_confirm(keyState);
            }
        }
    }
    inline void clear_keyState(const AllocatorId _alloc)
    {
        m_traversal.init();
        for(uint32_t k = 0; k < Keys; k++)
        {
            KeyAssignment* keyState = &m_keyState[k];
            // find allocated keys
            if(keyState->m_origin == _alloc)
            {
                keyUp_apply(keyState);
                keyUp_confirm(keyState);
            }
        }
    }
};
