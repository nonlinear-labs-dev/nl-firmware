#pragma once

/******************************************************************************/
/** @file       smoother_handle.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      provide smoothing functionality and abstraction in order to
                perform smoothing within a section
    @note       in theory, we could consider using parallel_data types for the
                renderers (which maybe would boost performance?)
    @todo
*******************************************************************************/

#include <stdint.h>

// basic smoothing unit
class ProtoSmoother
{
public:
    // smoothed value
    float m_value = 0.0f;
    // constructor
    inline ProtoSmoother()
    {}
    // starting a segment (_dx: converted time, _dest: destination)
    inline void start(const float _dx, const float _dest)
    {
        m_state = true;
        m_start = m_value;
        m_diff = _dest - m_start;
        m_x = m_dx = _dx;
    }
    // rendering
    inline void render()
    {
        if(m_state)
        {
            if(m_x >= 1.0f)
            {
                m_state = false;
                m_x = 1.0f;
            }
            m_value = m_start + (m_x * m_diff);
            m_x += m_dx;
        }
    }
    // reset (if needed)
    inline void reset()
    {
        m_state = false;
        m_value = 0.0f;
    }
private:
    // segment-specific private variables
    float m_start, m_diff, m_x, m_dx = 0.0f;
    bool m_state = false;
};

// smoother copy handle (for parameter smoothers that directly translate to signals)
template<uint32_t Length>
struct SmootherCopyHandle
{
    uint32_t m_smootherId[Length], m_signalId[Length], m_length = {};
    inline void add_copy_id(const uint32_t _smootherId, const uint32_t _signalId)
    {
        m_smootherId[m_length] = _smootherId;
        m_signalId[m_length] = _signalId;
        m_length++;
    }
};

// smoother handle abstraction (includes smoother arrays for each clock type)
template<class Sync, class Audio, class Fast, class Slow>
class SmootherHandle
{
public:
    // copy handles
    SmootherCopyHandle<static_cast<uint32_t>(Audio::_LENGTH_)> m_copy_audio;
    SmootherCopyHandle<static_cast<uint32_t>(Fast::_LENGTH_)> m_copy_fast;
    SmootherCopyHandle<static_cast<uint32_t>(Slow::_LENGTH_)> m_copy_slow;
    // constructor
    inline SmootherHandle()
    {}
    // start methods according to clock type
    inline void start_sync(const uint32_t _id, const float _dest)
    {
        m_sync[_id] = _dest;
    }
    inline void start_audio(const uint32_t _id, const float _dx, const float _dest)
    {
        m_audio[_id].start(_dx, _dest);
    }
    inline void start_fast(const uint32_t _id, const float _dx, const float _dest)
    {
        m_fast[_id].start(_dx, _dest);
    }
    inline void start_slow(const uint32_t _id, const float _dx, const float _dest)
    {
        m_slow[_id].start(_dx, _dest);
    }
    // render methods according to clock type
    inline void render_audio()
    {
        for(uint32_t i = 0; i < static_cast<uint32_t>(Audio::_LENGTH_); i++)
        {
            m_audio[i].render();
        }
    }
    inline void render_fast()
    {
        for(uint32_t i = 0; i < static_cast<uint32_t>(Fast::_LENGTH_); i++)
        {
            m_fast[i].render();
        }
    }
    inline void render_slow()
    {
        for(uint32_t i = 0; i < static_cast<uint32_t>(Slow::_LENGTH_); i++)
        {
            m_slow[i].render();
        }
    }
    // get methods according to clock type (in case of run-time-related access)
    inline float get_sync(const uint32_t _id)
    {
        return m_sync[_id];
    }
    inline float get_audio(const uint32_t _id)
    {
        return m_audio[_id].m_value;
    }
    inline float get_fast(const uint32_t _id)
    {
        return m_fast[_id].m_value;
    }
    inline float get_slow(const uint32_t _id)
    {
        return m_slow[_id].m_value;
    }
    // get method (auto-deducing clock type, in case of compile-time-related access)
    inline float get(const Sync _id)
    {
        return m_sync[static_cast<uint32_t>(_id)];
    }
    inline float get(const Audio _id)
    {
        return m_audio[static_cast<uint32_t>(_id)].m_value;
    }
    inline float get(const Fast _id)
    {
        return m_fast[static_cast<uint32_t>(_id)].m_value;
    }
    inline float get(const Slow _id)
    {
        return m_slow[static_cast<uint32_t>(_id)].m_value;
    }
    // reset method (if needed)
    inline void reset()
    {
        for(uint32_t i = 0; i < static_cast<uint32_t>(Sync::_LENGTH_); i++)
        {
            m_sync[i] = 0.0f;
        }
        for(uint32_t i = 0; i < static_cast<uint32_t>(Audio::_LENGTH_); i++)
        {
            m_audio[i].reset();
        }
        for(uint32_t i = 0; i < static_cast<uint32_t>(Fast::_LENGTH_); i++)
        {
            m_fast[i].reset();
        }
        for(uint32_t i = 0; i < static_cast<uint32_t>(Slow::_LENGTH_); i++)
        {
            m_slow[i].reset();
        }
    }
private:
    // private smoother arrays for each clock type
    ProtoSmoother
        m_audio[static_cast<uint32_t>(Audio::_LENGTH_)],
        m_fast[static_cast<uint32_t>(Fast::_LENGTH_)],
        m_slow[static_cast<uint32_t>(Slow::_LENGTH_)] = {};
    float m_sync[static_cast<uint32_t>(Sync::_LENGTH_)] = {};
};
