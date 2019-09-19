/******************************************************************************/
/** @file       dsp_host_dual.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

#include "dsp_host_dual.h"

dsp_host_dual::dsp_host_dual()
{
    m_mainOut_L = m_mainOut_R = 0.0f;
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{}

void dsp_host_dual::onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1)
{}

void dsp_host_dual::render()
{}

void dsp_host_dual::reset()
{}
