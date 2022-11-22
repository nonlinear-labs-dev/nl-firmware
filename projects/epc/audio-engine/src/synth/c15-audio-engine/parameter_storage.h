#pragma once

/******************************************************************************/
/** @file       parameter_storage.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      abstractions for parameters
                (currently, all parameters operate completely event-based)
    @todo
*******************************************************************************/

#include <ParameterMessages.h>
#include "parameter_types.h"
#include "macro_assignment.h"

namespace Engine
{

  namespace Storage
  {

    namespace Detail
    {

      template <typename TP> struct ModChain
      {
        MC_Assignment<C15::Parameters::Macro_Controls, TP> m_assignment = {};
        Parameters::Modulateable m_modulateables[(size_t) TP::_LENGTH_] = {};
      };

      struct ModMatrix
      {
        Parameters::HardwareSource m_hardwareSources[C15::Parameters::num_of_Hardware_Sources] = {};
        Parameters::HardwareAmount m_hardwareAmounts[C15::Parameters::num_of_Hardware_Amounts] = {};
        Parameters::MacroControl m_macroControls[(uint32_t) C15::Parameters::Macro_Controls::_OPTIONS_] = {};
        Parameters::MacroTime m_macroTimes[(uint32_t) C15::Parameters::Macro_Times::_OPTIONS_] = {};
        inline void init()
        {
          // manually setting up constant return behaviors for bender, aftertouch
          m_hardwareSources[(size_t) C15::Parameters::Hardware_Sources::Bender].m_behavior
              = C15::Properties::HW_Return_Behavior::Center;
          m_hardwareSources[(size_t) C15::Parameters::Hardware_Sources::Aftertouch].m_behavior
              = C15::Properties::HW_Return_Behavior::Zero;
          for(uint32_t src = 0; src < C15::Parameters::num_of_Hardware_Sources; src++)
          {
            const uint32_t offset = src * C15::Parameters::num_of_Macro_Controls;
            m_hardwareSources[src].m_offset = offset;
            for(uint32_t mc = 0; mc < C15::Parameters::num_of_Macro_Controls; mc++)
              m_hardwareAmounts[offset + mc].m_sourceId = src;
          }
        }
      };

      template <typename TP, typename DP> struct ParamChain : public ModChain<TP>
      {
        Parameters::Unmodulateable m_unmodulateables[(size_t) DP::_LENGTH_] = {};
      };

    }  // namespace Engine::Storage::Detail

    struct GlobalParameterStorage : public Detail::ModMatrix
    {
      Detail::ParamChain<C15::Parameters::Global_Modulateables, C15::Parameters::Global_Unmodulateables> m_parameters
          = {};
    };

    struct LayerParameterStorage
        // temporary
        : public Detail::ParamChain<C15::Parameters::Local_Modulateables, C15::Parameters::Local_Unmodulateables>
    {
      Detail::ParamChain<C15::Parameters::Polyphonic_Modulateables, C15::Parameters::Polyphonic_Unmodulateables>
          m_polyphonic = {};
      Detail::ParamChain<C15::Parameters::Monophonic_Modulateables, C15::Parameters::Monophonic_Unmodulateables>
          m_monophonic = {};
    };

  }  //namespace Engine::Storage

}  // namespace Engine
