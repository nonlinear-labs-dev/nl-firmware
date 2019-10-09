#pragma once

/******************************************************************************/
/** @file       ae_global_section.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all global parameters and dsp
    @todo
*******************************************************************************/

#include "smoother_handle.h"
#include "ae_info.h"

// master (volume, tune), scale, test tone (freq, amp, follow_key), soft clip
class GlobalSection
{
public:
    GlobalSignals m_signals;
    float m_out_l, m_out_r;
    GlobalSection();
    void add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId);
    void add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId);
    void add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId);
    float key_position(const uint32_t _pos);
    void render_audio(const float _left, const float _right);
    void render_fast();
    void render_slow();
private:
    SmootherHandle<
        C15::Smoothers::Global_Sync, C15::Smoothers::Global_Audio, C15::Smoothers::Global_Fast, C15::Smoothers::Global_Slow
    > m_smoothers;
    void postProcess_audio();
    void postProcess_fast();
    void postProcess_slow();
};
