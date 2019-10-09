#pragma once

/******************************************************************************/
/** @file       ae_poly_section.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all polyphonic parameters and dsp
    @todo
*******************************************************************************/

#include "smoother_handle.h"
#include "ae_info.h"

class PolySection
{
public:
    PolySignals m_signals;
    PolySection();
    void add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId);
    void add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId);
    void add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId);
    void render_audio(const float _left, const float _right);
    void render_fast();
    void render_slow();
private:
    SmootherHandle<
        C15::Smoothers::Poly_Sync, C15::Smoothers::Poly_Audio, C15::Smoothers::Poly_Fast, C15::Smoothers::Poly_Slow
    > m_smoothers;
    void postProcess_audio();
    void postProcess_fast();
    void postProcess_slow();
};
