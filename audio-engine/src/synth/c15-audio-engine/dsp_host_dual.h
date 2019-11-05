#pragma once

/******************************************************************************/
/** @file       dsp_host_dual.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

#include "parameter-db/generated/c15_config.h"
#include "parameter-db/generated/parameter_list.h"
#include "parameter_handle.h"
#include "pe_exponentiator.h"
#include "voice_allocation.h"

#include "ae_global_section.h"
#include "ae_poly_section.h"
#include "ae_mono_section.h"
#include "ae_fadepoint.h"

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
  void onRawMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
  void render();
  void reset();

 private:
  void keyDown(const float _vel);
  void keyUp(const float _vel);
  ParameterHandle<C15::Properties::SmootherScale, C15::Descriptors::SmootherSection, C15::Descriptors::SmootherClock,
                  C15::Descriptors::ParameterSignal, C15::Properties::LayerId, C15::Parameters::Hardware_Sources,
                  C15::Parameters::Global_Parameters, C15::Parameters::Hardware_Amounts,
                  C15::Parameters::Macro_Controls, C15::Parameters::Modulateable_Parameters,
                  C15::Parameters::Unmodulateable_Parameters>
      m_params;
  // essential tools
  exponentiator m_convert;
  ClockHandle m_clock;
  TimeHandle m_time;
  // layer handling
  C15::Properties::LayerMode m_layer_mode;
  uint32_t m_layer_focus;  // probably obsolete
  // global dsp components
  GlobalSection m_global;
  VoiceAllocation<C15::Config::total_polyphony, C15::Config::local_polyphony, C15::Config::key_count> m_alloc;
  ae_fade_table m_fade;
  // layered dsp components
  PolySection m_poly[2];
  MonoSection m_mono[2];
  float scale(const C15::Properties::SmootherScale _id, const float _scaleFactor, const float _scaleOffset,
              float _value);
  // helper values
  const float m_format_vel = 4095.0f / 127.0f, m_format_hw = 8000.0f / 127.0f, m_format_pb = 8000.0f / 16383.0f,
              m_norm_vel = 1.0f / 4095.0f, m_norm_hw = 1.0f / 8000.0f;
  uint32_t m_key_pos = 0;
  bool m_key_valid = false;
};
