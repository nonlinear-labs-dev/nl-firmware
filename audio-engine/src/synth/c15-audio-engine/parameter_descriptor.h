#pragma once

/******************************************************************************/
/** @file       parameter_descriptor.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      combining all parameter definitions into flexible descriptors,
                allowing for more flexible parameter list
    @todo
*******************************************************************************/

#include <stdint.h>
#include "parameter_info.h"

namespace C15
{

// provide a "None" descriptor (for ignored TCD IDs)
const uint32_t None = 0;

namespace Descriptors
{

// providing a parameter type descriptor
enum class ParameterType
{
    None, Hardware_Source, Global_Parameter, Hardware_Amount, Macro_Control, Macro_Time, Modulateable_Parameter, Unmodulateable_Parameter
};

struct TypeDescriptor
{
    const ParameterType m_type;
    const uint32_t m_index;
    // constructors
    inline TypeDescriptor()
        : m_type(ParameterType::None)
        , m_index(None)
    {}
    inline TypeDescriptor(const uint32_t _param)
        : m_type(ParameterType::None)
        , m_index(_param)
    {}
    inline TypeDescriptor(const Parameters::Hardware_Sources _param)
        : m_type(ParameterType::Hardware_Source)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Global_Parameters _param)
        : m_type(ParameterType::Global_Parameter)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Hardware_Amounts _param)
        : m_type(ParameterType::Hardware_Amount)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Macro_Controls _param)
        : m_type(ParameterType::Macro_Control)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Macro_Times _param)
        : m_type(ParameterType::Macro_Time)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Modulateable_Parameters _param)
        : m_type(ParameterType::Modulateable_Parameter)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline TypeDescriptor(const Parameters::Unmodulateable_Parameters _param)
        : m_type(ParameterType::Unmodulateable_Parameter)
        , m_index(static_cast<uint32_t>(_param))
    {}
};

//providing a parameter smoother descriptor
enum class ParameterSmoother
{
    None,
    Global_Sync, Global_Audio, Global_Fast, Global_Slow,
    Poly_Sync, Poly_Audio, Poly_Fast, Poly_Slow,
    Mono_Sync, Mono_Audio, Mono_Fast, Mono_Slow
};

struct SmootherDescriptor
{
    const ParameterSmoother m_type;
    const uint32_t m_index;
    // constructors
    inline SmootherDescriptor()
        : m_type(ParameterSmoother::None)
        , m_index(None)
    {}
    inline SmootherDescriptor(const uint32_t _param)
        : m_type(ParameterSmoother::None)
        , m_index(_param)
    {}
    inline SmootherDescriptor(const Smoothers::Global_Sync _param)
        : m_type(ParameterSmoother::Global_Sync)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Global_Audio _param)
        : m_type(ParameterSmoother::Global_Audio)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Global_Fast _param)
        : m_type(ParameterSmoother::Global_Fast)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Global_Slow _param)
        : m_type(ParameterSmoother::Global_Slow)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Poly_Sync _param)
        : m_type(ParameterSmoother::Poly_Sync)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Poly_Audio _param)
        : m_type(ParameterSmoother::Poly_Audio)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Poly_Fast _param)
        : m_type(ParameterSmoother::Poly_Fast)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Poly_Slow _param)
        : m_type(ParameterSmoother::Poly_Slow)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Mono_Sync _param)
        : m_type(ParameterSmoother::Mono_Sync)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Mono_Audio _param)
        : m_type(ParameterSmoother::Mono_Audio)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Mono_Fast _param)
        : m_type(ParameterSmoother::Mono_Fast)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SmootherDescriptor(const Smoothers::Mono_Slow _param)
        : m_type(ParameterSmoother::Mono_Slow)
        , m_index(static_cast<uint32_t>(_param))
    {}
};

//providing a parameter signal descriptor
enum class ParameterSignal
{
    None, Global_Signal, Quasipoly_Signal, Truepoly_Signal, Mono_Signal
};

struct SignalDescriptor
{
    const ParameterSignal m_signal;
    const uint32_t m_index;
    // constructors
    inline SignalDescriptor()
        : m_signal(ParameterSignal::None)
        , m_index(None)
    {}
    inline SignalDescriptor(const uint32_t _param)
        : m_signal(ParameterSignal::None)
        , m_index(_param)
    {}
    inline SignalDescriptor(const Signals::Global_Signals _param)
        : m_signal(ParameterSignal::Global_Signal)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SignalDescriptor(const Signals::Quasipoly_Signals _param)
        : m_signal(ParameterSignal::Quasipoly_Signal)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SignalDescriptor(const Signals::Truepoly_Signals _param)
        : m_signal(ParameterSignal::Truepoly_Signal)
        , m_index(static_cast<uint32_t>(_param))
    {}
    inline SignalDescriptor(const Signals::Mono_Signals _param)
        : m_signal(ParameterSignal::Mono_Signal)
        , m_index(static_cast<uint32_t>(_param))
    {}
};

} // namespace C15::Descriptors

struct ParameterDescriptor
{
    Descriptors::TypeDescriptor m_param;
    Scale m_scaleId = Scale::None;
    float m_scaleArg = 0.0f;
    uint32_t m_polarity = 0;
    Descriptors::SmootherDescriptor m_smoother;
    Descriptors::SignalDescriptor m_signal;
};

} // namespace C15
