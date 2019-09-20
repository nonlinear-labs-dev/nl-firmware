#pragma once

/******************************************************************************/
/** @file       parameter_handle.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main parameter handle
    @todo
*******************************************************************************/

#include "parameter_storage.h"
#include "parameter_list.h"

template<typename Scale, typename Clock, typename Signal, class Layer, class HW, class GP, class HA, class MC, class TP, class DP>
struct ParameterHandle
{
    // numeric getters (when dispatching TCD ids)
    inline Hardware_Source_Parameter* get_source(const uint32_t _id)
    {
        return &m_global.m_source[_id];
    }
    inline Unmodulateable_Parameter<Scale, Clock, Signal>* get_global(const uint32_t _id)
    {
        return &m_global.m_direct[_id];
    }
    inline Hardware_Amount_Parameter* get_amount(const uint32_t _layerId, const uint32_t _id)
    {
        return &m_layer[_layerId].m_amount[_id];
    }
    inline Macro_Control_Parameter* get_macro(const uint32_t _layerId, const uint32_t _id)
    {
        return &m_layer[_layerId].m_macro[_id];
    }
    inline Modulateable_Parameter<Scale, Clock, Signal>* get_target(const uint32_t _layerId, const uint32_t _id)
    {
        return &m_layer[_layerId].m_target[_id];
    }
    inline Unmodulateable_Parameter<Scale, Clock, Signal>* get_direct(const uint32_t _layerId, const uint32_t _id)
    {
        return &m_layer[_layerId].m_direct[_id];
    }
    // enum-class-bassed getters (if needed)
    inline Hardware_Source_Parameter* get(const HW _id)
    {
        return &m_global.m_source[static_cast<uint32_t>(_id)];
    }
    inline Unmodulateable_Parameter<Scale, Clock, Signal>* get(const GP _id)
    {
        return &m_global.m_direct[static_cast<uint32_t>(_id)];
    }
    inline Hardware_Amount_Parameter* get(const Layer _layerId, const HA _id)
    {
        return &m_layer[static_cast<uint32_t>(_layerId)].m_amount[static_cast<uint32_t>(_id)];
    }
    inline Macro_Control_Parameter* get(const Layer _layerId, const MC _id)
    {
        return &m_layer[static_cast<uint32_t>(_layerId)].m_macro[static_cast<uint32_t>(_id)];
    }
    inline Modulateable_Parameter<Scale, Clock, Signal>* get(const Layer _layerId, const TP _id)
    {
        return &m_layer[static_cast<uint32_t>(_layerId)].m_target[static_cast<uint32_t>(_id)];
    }
    inline Unmodulateable_Parameter<Scale, Clock, Signal>* get(const Layer _layerId, const DP _id)
    {
        return &m_layer[static_cast<uint32_t>(_layerId)].m_direct[static_cast<uint32_t>(_id)];
    }
    // members: layered and global parameter storage
    Layer_Parameter_Storage<
        Scale, Clock, Signal, C15::Parameters::Hardware_Amounts, C15::Parameters::Macro_Controls,
        C15::Parameters::Modulateable_Parameters, C15::Parameters::Unmodulateable_Parameters
    > m_layer[static_cast<uint32_t>(Layer::_LENGTH_)];
    Global_Parameter_Storage<
        Scale, Clock, Signal, C15::Parameters::Hardware_Sources, C15::Parameters::Global_Parameters
    > m_global;
};
