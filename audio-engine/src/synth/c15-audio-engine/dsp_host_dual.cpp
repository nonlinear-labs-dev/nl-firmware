#include "dsp_host_dual.h"

/******************************************************************************/
/** @file       dsp_host_dual.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

dsp_host_dual::dsp_host_dual()
{
    m_mainOut_L = m_mainOut_R = 0.0f;
    m_focus = 1;
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{
    m_clock.init(_samplerate);
    m_time.init(_samplerate);
    // init parameters by parameter list
    for(uint32_t i = 0; i < C15::Config::tcd_elements; i++)
    {
        auto element = C15::ParameterList[i];
        switch(element.m_param.m_type)
        {
        case C15::Descriptors::ParameterType::Global_Parameter:
            m_params.init_global(element);
            switch(element.m_signal.m_signal)
            {
            case C15::Descriptors::ParameterSignal::Global_Signal:
                switch(element.m_smoother.m_clock)
                {
                case C15::Descriptors::SmootherClock::Audio:
                    m_global.add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                case C15::Descriptors::SmootherClock::Fast:
                    m_global.add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                case C15::Descriptors::SmootherClock::Slow:
                    m_global.add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                }
                break;
            }
            break;
        case C15::Descriptors::ParameterType::Macro_Control:
            m_params.init_macro(element);
            break;
        case C15::Descriptors::ParameterType::Modulateable_Parameter:
            m_params.init_modulateable(element);
            switch(element.m_signal.m_signal)
            {
            case C15::Descriptors::ParameterSignal::Quasipoly_Signal:
                switch(element.m_smoother.m_clock)
                {
                case C15::Descriptors::SmootherClock::Audio:
                    m_poly[0].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Fast:
                    m_poly[0].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Slow:
                    m_poly[0].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                }
                break;
            case C15::Descriptors::ParameterSignal::Mono_Signal:
                switch(element.m_smoother.m_clock)
                {
                case C15::Descriptors::SmootherClock::Audio:
                    m_mono[0].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Fast:
                    m_mono[0].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Slow:
                    m_mono[0].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                }
                break;
            }
            break;
        case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
            m_params.init_unmodulateable(element);
            switch(element.m_signal.m_signal)
            {
            case C15::Descriptors::ParameterSignal::Quasipoly_Signal:
                switch(element.m_smoother.m_clock)
                {
                case C15::Descriptors::SmootherClock::Audio:
                    m_poly[0].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Fast:
                    m_poly[0].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Slow:
                    m_poly[0].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_poly[1].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                }
                break;
            case C15::Descriptors::ParameterSignal::Mono_Signal:
                switch(element.m_smoother.m_clock)
                {
                case C15::Descriptors::SmootherClock::Audio:
                    m_mono[0].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_audio_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Fast:
                    m_mono[0].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_fast_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                case C15::Descriptors::SmootherClock::Slow:
                    m_mono[0].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    m_mono[1].add_copy_slow_id(element.m_smoother.m_index, element.m_signal.m_index);
                    break;
                }
                break;
            }
            break;
        }
    }
}

void dsp_host_dual::onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1)
{}

void dsp_host_dual::render()
{
    m_clock.render();
    // slow rendering
    if(m_clock.m_slow)
    {}
    // fast rendering
    if(m_clock.m_fast)
    {}
    // audio rendering
}

void dsp_host_dual::reset()
{}
