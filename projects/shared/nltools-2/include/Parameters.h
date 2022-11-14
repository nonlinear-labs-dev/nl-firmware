#pragma once


#include <parameter_list.h>
#include <nltools/messaging/Message.h>

namespace nltools
{

    namespace controls
    {

        using Id = C15::PID::ParameterID;               // note: may be reducible
        using Position = double;                        // note: unfortunately not generic
        using Type = C15::Descriptors::ParameterType;

        // generic structures

        namespace detail {

            // note: may be reducible
            struct Control {
                Id m_id = Id::None;
            };

            template<Type T>
            struct Parameter : public Control
            {
                static_assert(T != Type::None, "Parameter cannot be of type None");
                static constexpr Type getParameterType()
                {
                    return T;
                }
                static inline bool compareParameter(const Parameter &_lhs, const Parameter &_rhs) {
                    auto ret = _lhs.m_id == _rhs.m_id;
                    ret &= _lhs.m_controlPosition == _rhs.m_controlPosition;
                    return ret;
                }
                bool validateParameterType() const
                {
                    return C15::ParameterList[m_id].m_param.m_type == getParameterType();
                }
                Position m_controlPosition = {};
            };

            template<Type T>
            struct HardwareSource : public Parameter<T>
            {
                static_assert(
                    T == Type::Hardware_Source || T == Type::Display_Parameter,
                    "HardwareSource type is restricted"
                );
                    static inline bool compareHardwareSource(const HardwareSource &_lhs, const HardwareSource &_rhs)
                    {
                        auto ret = Parameter<T>::compareParameter(_lhs, _rhs);
                        ret &= _lhs.m_returnMode == _rhs.m_returnMode;
                        return ret;
                    }

                ReturnMode m_returnMode = ReturnMode::None;
            };

            template<Type T>
            struct Modulateable : public Parameter<T>
            {
                static_assert(
                    T == Type::Global_Modulateable || T == Type::Local_Modulateable ||
                    T == Type::Polyphonic_Modulateable || T == Type::Monophonic_Modulateable,
                    "Modulateable type is restricted"
                );
                    static inline bool compareModulateable(const Modulateable &_lhs, const Modulateable &_rhs) {
                        auto ret = Parameter<T>::compareParameter(_lhs, _rhs);
                        ret &= _lhs.m_macro == _rhs.m_macro;
                        ret &= _lhs.m_modulationAmount == _rhs.m_modulationAmount;
                        return ret;
                    }

                MacroControls m_macro = MacroControls::NONE;
                Position m_modulationAmount = {};
            };

            template<Type T>
            struct Unmodulateable : public Parameter<T>
            {
                static_assert(
                    T == Type::Global_Unmodulateable || T == Type::Local_Unmodulateable ||
                    T == Type::Polyphonic_Unmodulateable || T == Type::Monophonic_Unmodulateable,
                    "Unmodulateable type is restricted"
                );
            };

        } // namespace nltools::controls::detail

        // usable structures

        struct HardwareSourceParameter : public detail::HardwareSource<Type::Hardware_Source>
        {};

        inline bool operator==(const HardwareSourceParameter &_lhs, const HardwareSourceParameter &_rhs)
        {
            return detail::HardwareSource<Type::Hardware_Source>::compareHardwareSource(_lhs, _rhs);
        }

        struct HardwareSourceSendParameter : public detail::HardwareSource<Type::Display_Parameter>
        {};

        inline bool operator==(const HardwareSourceSendParameter &_lhs, const HardwareSourceSendParameter &_rhs)
        {
            return detail::HardwareSource<Type::Display_Parameter>::compareHardwareSource(_lhs, _rhs);
        }

        struct HardwareAmountParameter : public detail::Parameter<Type::Hardware_Amount>
        {};

        inline bool operator==(const HardwareAmountParameter &_lhs, const HardwareAmountParameter &_rhs)
        {
            return detail::Parameter<Type::Hardware_Amount>::compareParameter(_lhs, _rhs);
        }

        struct MacroControlParameter : public detail::Parameter<Type::Macro_Control>
        {};

        inline bool operator==(const MacroControlParameter &_lhs, const MacroControlParameter &_rhs)
        {
            return detail::Parameter<Type::Macro_Control>::compareParameter(_lhs, _rhs);
        }

        struct MacroTimeParameter : public detail::Parameter<Type::Macro_Time>
        {};

        inline bool operator==(const MacroTimeParameter &_lhs, const MacroTimeParameter &_rhs)
        {
            return detail::Parameter<Type::Macro_Time>::compareParameter(_lhs, _rhs);
        }

        struct GlobalModulateableParameter : public detail::Modulateable<Type::Global_Modulateable>
        {};

        inline bool operator==(const GlobalModulateableParameter &_lhs, const GlobalModulateableParameter &_rhs)
        {
            return detail::Modulateable<Type::Global_Modulateable>::compareModulateable(_lhs, _rhs);
        }

        struct GlobalUnmodulateableParameter : public detail::Unmodulateable<Type::Global_Unmodulateable>
        {};

        inline bool operator==(const GlobalUnmodulateableParameter &_lhs, const GlobalUnmodulateableParameter &_rhs)
        {
            return detail::Unmodulateable<Type::Global_Unmodulateable>::compareParameter(_lhs, _rhs);
        }

        // todo: deprecate
        struct LocalModulateableParameter : public detail::Modulateable<Type::Local_Modulateable>
        {};

        inline bool operator==(const LocalModulateableParameter &_lhs, const LocalModulateableParameter &_rhs)
        {
            return detail::Modulateable<Type::Local_Modulateable>::compareModulateable(_lhs, _rhs);
        }

        // todo: deprecate
        struct LocalUnmodulateableParameter : public detail::Unmodulateable<Type::Local_Unmodulateable>
        {};

        inline bool operator==(const LocalUnmodulateableParameter &_lhs, const LocalUnmodulateableParameter &_rhs)
        {
            return detail::Unmodulateable<Type::Local_Unmodulateable>::compareParameter(_lhs, _rhs);
        }

        struct PolyphonicModulateableParameter : public detail::Modulateable<Type::Polyphonic_Modulateable>
        {};

        inline bool operator==(const PolyphonicModulateableParameter &_lhs, const PolyphonicModulateableParameter &_rhs)
        {
            return detail::Modulateable<Type::Polyphonic_Modulateable>::compareModulateable(_lhs, _rhs);
        }

        struct PolyphonicUnmodulateableParameter : public detail::Unmodulateable<Type::Polyphonic_Unmodulateable>
        {};

        inline bool operator==(const PolyphonicUnmodulateableParameter &_lhs, const PolyphonicUnmodulateableParameter &_rhs)
        {
            return detail::Unmodulateable<Type::Polyphonic_Unmodulateable>::compareParameter(_lhs, _rhs);
        }

        struct MonophonicModulateableParameter : public detail::Modulateable<Type::Monophonic_Modulateable>
        {};

        inline bool operator==(const MonophonicModulateableParameter &_lhs, const MonophonicModulateableParameter &_rhs)
        {
            return detail::Modulateable<Type::Monophonic_Modulateable>::compareModulateable(_lhs, _rhs);
        }

        struct MonophonicUnmodulateableParameter : public detail::Unmodulateable<Type::Monophonic_Unmodulateable>
        {};

        inline bool operator==(const MonophonicUnmodulateableParameter &_lhs, const MonophonicUnmodulateableParameter &_rhs)
        {
            return detail::Unmodulateable<Type::Monophonic_Unmodulateable>::compareParameter(_lhs, _rhs);
        }

    } // namespace nltools::controls

} // namespace nltools
