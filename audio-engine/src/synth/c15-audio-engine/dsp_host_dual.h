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
#include <nltools/messaging/Message.h>

#include "parameter_handle.h"
#include "pe_exponentiator.h"
#include "voice_allocation.h"

#include "ae_global_section.h"
#include "ae_poly_section.h"
#include "ae_mono_section.h"
#include "ae_fadepoint.h"

// basic logging switches
#define LOG_INIT 0
#define LOG_MIDI 0
#define LOG_DISPATCH 0
#define LOG_EDITS 0
#define LOG_SETTINGS 0
#define LOG_RECALL 1
#define LOG_TIMES 0
#define LOG_KEYS 0
#define LOG_TRANSITIONS 0
#define LOG_RESET 0
// engine safety
#define IGNORE_MONO_GROUP 1

class dsp_host_dual
{
 public:
  // public members
  float m_mainOut_R, m_mainOut_L;
  // constructor
  dsp_host_dual();
  // public methods
  void init(const uint32_t _samplerate, const uint32_t _polyphony);
  // handles for inconvenient stuff
  C15::Properties::HW_Return_Behavior getBehavior(const ReturnMode _mode);
  C15::Properties::HW_Return_Behavior getBehavior(const RibbonReturnMode _mode);
  C15::Parameters::Macro_Controls getMacro(const MacroControls _mc);
  // event bindings
  void onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
  void onRawMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
  void onPresetMessage(const nltools::msg::SinglePresetMessage &_msg);
  void onPresetMessage(const nltools::msg::SplitPresetMessage &_msg);
  void onPresetMessage(const nltools::msg::LayerPresetMessage &_msg);
  void update_event_hw_source(const uint32_t _index, const bool _lock,
                              const C15::Properties::HW_Return_Behavior _behavior, const float _position);
  void update_event_hw_amount(const uint32_t _index, const uint32_t _layer, const bool _lock, const float _position);
  void update_event_macro_ctrl(const uint32_t _index, const uint32_t _layer, const bool _lock, const float _position);
  void update_event_macro_time(const uint32_t _index, const uint32_t _layer, const bool _lock, const float _position);
  void update_event_direct_param(const uint32_t _index, const uint32_t _layer, const bool _lock, const float _position);
  void update_event_target_param(const uint32_t _index, const uint32_t _layer, const bool _lock, const float _position,
                                 const C15::Parameters::Macro_Controls _source, const float _amount);
  void update_event_global_param(const uint32_t _index, const bool _lock, const float _position);
  void update_event_edit_time(const float _position);
  void update_event_transition_time(const float _position);
  void update_event_note_shift(const float _shift);
  void update_event_glitch_suppr(const bool _enabled);
  void render();
  void reset();

 private:
  void keyDown(const float _vel);
  void keyUp(const float _vel);
  float scale(const Parameter_Scale<C15::Properties::SmootherScale> _scl, float _value);
  void update_event_time(Time_Parameter *_param, const float _ms);
  void mod_event_mc_chain(const uint32_t _index, const uint32_t _layer, const float _position,
                          const Time_Parameter _time);
  void transition_event(const uint32_t _id, const uint32_t _layer, const Time_Parameter _time,
                        const C15::Descriptors::SmootherSection _section, const C15::Descriptors::SmootherClock _clock,
                        const float _dest);
  void transition_event(const uint32_t _id, const Time_Parameter _time,
                        const C15::Descriptors::SmootherSection _section, const C15::Descriptors::SmootherClock _clock,
                        const float _dest);
  bool recall_event_changed(const C15::Properties::LayerId _layerId, const float _value);
  void recall_event_global(const nltools::msg::ParameterGroups::PedalParameter &_source);
  void recall_event_global(const nltools::msg::ParameterGroups::BenderParameter &_source);
  void recall_event_global(const nltools::msg::ParameterGroups::AftertouchParameter &_source);
  void recall_event_global(const nltools::msg::ParameterGroups::RibbonParameter &_source);
  void recall_event_global(const nltools::msg::ParameterGroups::Parameter &_source);
  void recall_event_local(const uint32_t _layer, const nltools::msg::ParameterGroups::MonoParameter &_source);
  void recall_event_local(const uint32_t _layer, const nltools::msg::ParameterGroups::MacroParameter &_source);
  void recall_event_local(const uint32_t _layer, const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_source);
  void recall_event_local(const uint32_t _layer, const nltools::msg::ParameterGroups::ModulateableParameter &_source);
  void recall_event_direct_param(const uint32_t _layer, const uint32_t _index, const float _position);
  void recall_event_target_param(const uint32_t _layer, const uint32_t _index, const float _position,
                                 const C15::Parameters::Macro_Controls _source, const float _amount);
  void recall_event_target_aspects(const uint32_t _layer, const uint32_t _index);
  void recall_event_single();
  // preloadable preset buffers
  nltools::msg::SinglePresetMessage m_preloaded_single_data;
  nltools::msg::SplitPresetMessage m_preloaded_split_data;
  nltools::msg::LayerPresetMessage m_preloaded_layer_data;
  // parameters
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
  Time_Parameter m_edit_time, m_transition_time;
  const Parameter_Scale<C15::Properties::SmootherScale> m_transition_scale
      = { C15::Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f };
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
  // helper values
  const float m_format_vel = 4095.0f / 127.0f, m_format_hw = 8000.0f / 127.0f, m_format_pb = 8000.0f / 16383.0f,
              m_norm_vel = 1.0f / 4095.0f, m_norm_hw = 1.0f / 8000.0f;
  uint32_t m_key_pos = 0;
  bool m_key_valid = false, m_layer_changed = false, m_glitch_suppression = false;
};
