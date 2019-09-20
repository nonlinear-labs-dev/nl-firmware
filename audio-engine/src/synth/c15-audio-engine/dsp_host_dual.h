#pragma once

/******************************************************************************/
/** @file       dsp_host_dual.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

#include "c15_config.h"
#include "parameter_list.h"
#include "smoother_handle.h"
#include "macro_assignment.h"
#include "signal_storage_dual.h"
#include "parameter_handle.h"

class dsp_host_dual
{
public:
    // public members
    float m_mainOut_R, m_mainOut_L;
    // constructor
    dsp_host_dual();
    // public methods
    void init(const uint32_t _samplerate, const uint32_t _polyphony);
    void onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
    void render();
    void reset();
private:
};
