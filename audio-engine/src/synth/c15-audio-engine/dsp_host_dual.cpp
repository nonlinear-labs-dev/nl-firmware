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
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{
    const float samplerate = static_cast<float>(_samplerate);
    m_va.init();
    m_convert.init();
    m_clock.init(_samplerate);
    m_time.init(_samplerate);
    m_fade.init(samplerate);
    // init parameters by parameter list
    for(uint32_t i = 0; i < C15::Config::tcd_elements; i++)
    {
        auto element = C15::ParameterList[i];
        switch(element.m_param.m_type)
        {
        // global parameters need their properties and can directly feed to global signals
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
        // macro controls need to know their position in mod matrix (which hw amounts are relevant)
        case C15::Descriptors::ParameterType::Macro_Control:
            m_params.init_macro(element);
            break;
        // (local) modulateable parameters need their properties and can directly feed to either quasipoly or mono signals
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
        // (local) unmodulateable parameters need their properties and can directly feed to either quasipoly or mono signals
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
    // audio rendering (always)
}

void dsp_host_dual::reset()
{}

// taken from milestone 1.5 (as only modulateable and unmodulateable parameters need scaling, this should suffice)
float dsp_host_dual::scale(const C15::Properties::Scale _id, const float _scaleArg, float _value)
{
    float result = 0.0f;
    switch(_id)
    {
    case C15::Properties::Scale::None:
        break;
    case C15::Properties::Scale::Linear_Offset:
        result = _value + _scaleArg;
        break;
    case C15::Properties::Scale::Linear_Factor:
        result = _value * _scaleArg;
        break;
    case C15::Properties::Scale::Linear_Inverse:
        result = _scaleArg - _value;
        break;
    case C15::Properties::Scale::Parabolic_Offset:
        result = (std::abs(_value) * _value) + _scaleArg;
        break;
    case C15::Properties::Scale::Parabolic_Factor:
        result = std::abs(_value) * _value * _scaleArg;
        break;
    case C15::Properties::Scale::Cubic_Offset:
        result = (_value * _value * _value) + _scaleArg;
        break;
    case C15::Properties::Scale::S_Curve_Inverse:
        _value = (2.0f * (_scaleArg - _value)) - 1.0f;
        result = (_value * _value * _value * -0.25f) + (_value * 0.75f) + 0.5f;
        break;
    case C15::Properties::Scale::Expon_Gain:
        result = m_convert.eval_level(_value + _scaleArg);
        break;
    case C15::Properties::Scale::Expon_Osc_Pitch:
        result = m_convert.eval_osc_pitch(_value + _scaleArg);
        break;
    case C15::Properties::Scale::Expon_Lin_Pitch:
        result = m_convert.eval_lin_pitch(_value + _scaleArg);
        break;
    case C15::Properties::Scale::Expon_Shaper_Drive:
        result = (m_convert.eval_level(_value) * _scaleArg) - _scaleArg;
        break;
    case C15::Properties::Scale::Expon_Mix_Drive:
        result = m_convert.eval_level(_value) * _scaleArg;
        break;
//    case C15::Properties::Scale::Expon_Cab_Drive: // seems obsolete, equals normal expon_gain scaling
//        result = m_convert.eval_level(_value + _scaleArg);
//        break;
    case C15::Properties::Scale::Expon_Env_Time:
        result = m_convert.eval_time((_value * _scaleArg * 104.0781f) - 20.0f);
        break;
    }
    return result;
}
