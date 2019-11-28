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
#define LOG_MISSING 1
#define LOG_FAIL 1
#define LOG_INIT 1
#define LOG_MIDI 0
#define LOG_DISPATCH 0
#define LOG_EDITS 1
#define LOG_TIMES 1
#define LOG_SETTINGS 1
#define LOG_RECALL 1
#define LOG_KEYS 1
#define LOG_KEYS_POLY 1
#define LOG_TRANSITIONS 0
#define LOG_RESET 1

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
  C15::ParameterDescriptor getParameter(const int _id);
  // event bindings: debug
  void logStatus();
  // event bindings: LPC or MIDI Device (in Dev_PC mode)
  void onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
  void onRawMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1);
  // event bindings: Preset Messages
  void onPresetMessage(const nltools::msg::SinglePresetMessage &_msg);
  void onPresetMessage(const nltools::msg::SplitPresetMessage &_msg);
  void onPresetMessage(const nltools::msg::LayerPresetMessage &_msg);
  // event bindings: Parameter Changed Messages
  void globalParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg);
  void globalParChg(const uint32_t _id, const nltools::msg::HWSourceChangedMessage &_msg);
  void localParChg(const uint32_t _id, const nltools::msg::HWAmountChangedMessage &_msg);
  void localParChg(const uint32_t _id, const nltools::msg::MacroControlChangedMessage &_msg);
  void localParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg);
  void localParChg(const uint32_t _id, const nltools::msg::ModulateableParameterChangedMessage &_msg);
  void localTimeChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg);
  void localUnisonChg(const nltools::msg::UnmodulateableParameterChangedMessage &_msg);
  void onSettingEditTime(const float _position);
  void onSettingTransitionTime(const float _position);
  void onSettingNoteShift(const float _shift);
  void onSettingGlitchSuppr(const bool _enabled);
  void onSettingInitialSinglePreset();
  uint32_t onSettingToneToggle();
  void render();
  void reset();

 private:
  // preloadable preset buffers
  nltools::msg::SinglePresetMessage m_preloaded_single_data;
  nltools::msg::SplitPresetMessage m_preloaded_split_data;
  nltools::msg::LayerPresetMessage m_preloaded_layer_data;
  // parameters
  Engine::Param_Handle m_params;
  Time_Param m_edit_time, m_transition_time;
  Setting_Param m_reference;
  const C15::ParameterDescriptor m_invalid_param = { C15::None };
  // essential tools
  exponentiator m_convert;
  Engine::Handle::Clock_Handle m_clock;
  Engine::Handle::Time_Handle m_time;
  // layer handling
  C15::Properties::LayerMode m_layer_mode, m_preloaded_layer_mode;
  uint32_t m_layer_focus;  // probably obsolete
  // global dsp components
  GlobalSection m_global;
  VoiceAllocation<C15::Config::total_polyphony, C15::Config::local_polyphony, C15::Config::key_count> m_alloc;
  // dsp components
  ae_fade_table m_fade;
  ae_fader m_output_mute;
  PolySection m_poly[2];
  MonoSection m_mono[2];
  LayerSignalCollection m_z_layers[2];
  // helper values
  const float m_format_vel = 4095.0f / 127.0f, m_format_hw = 8000.0f / 127.0f, m_format_pb = 8000.0f / 16383.0f,
              m_norm_vel = 1.0f / 4095.0f, m_norm_hw = 1.0f / 8000.0f;
  uint32_t m_key_pos = 0, m_tone_state = 0;
  bool m_key_valid = false, m_layer_changed = false, m_glitch_suppression = false;
  // handles for inconvenient stuff
  C15::Properties::HW_Return_Behavior getBehavior(const ReturnMode _mode);
  C15::Properties::HW_Return_Behavior getBehavior(const RibbonReturnMode _mode);
  C15::Parameters::Macro_Controls getMacro(const MacroControls _mc);
  uint32_t getMacroId(const MacroControls _mc);
  C15::Properties::LayerId getLayer(const VoiceGroup _vg);
  uint32_t getLayerId(const VoiceGroup _vg);
  // key events
  void keyDown(const float _vel);
  void keyUp(const float _vel);
  float scale(const Scale_Aspect _scl, float _value);
  // inner event flow
  void updateHW(const uint32_t _id, const uint32_t _raw);
  void updateTime(Time_Aspect *_param, const float _ms);
  void updateModChain(Macro_Param *_mc);
  void updateModChain(const uint32_t _layer, Macro_Param *_mc);
  void globalTransition(const Direct_Param *_param, const Time_Aspect _time);
  void localTransition(const uint32_t _layer, const Direct_Param *_param, const Time_Aspect _time);
  void localTransition(const uint32_t _layer, const Target_Param *_param, const Time_Aspect _time);
  void evalFadePoint();
  Direct_Param *evalVoiceChg(const C15::Properties::LayerId _layerId,
                             const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_unisonVoices);
  void recallSingle();
  void recallSplit();
  void recallLayer();
  void globalParRcl(const nltools::msg::ParameterGroups::PedalParameter &_source);
  void globalParRcl(const nltools::msg::ParameterGroups::BenderParameter &_source);
  void globalParRcl(const nltools::msg::ParameterGroups::AftertouchParameter &_source);
  void globalParRcl(const nltools::msg::ParameterGroups::RibbonParameter &_source);
  void globalParRcl(const nltools::msg::ParameterGroups::Parameter &_source);
  void localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_source);
  void localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::MacroParameter &_source);
  //void localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::MonoParameter &_source);
  void localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::ModulateableParameter &_source);
  void localTimeRcl(const uint32_t _layer, const uint32_t _id, const float _value);
  void localDirectRcl(Direct_Param *_param, const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_source);
  //void localDirectRcl(Direct_Param *_param, const nltools::msg::ParameterGroups::MonoParameter &_source);
  void localTargetRcl(Target_Param *_param, const nltools::msg::ParameterGroups::ModulateableParameter &_source);
  //void localTargetRcl(Target_Param *_param, const nltools::msg::ParameterGroups::MonoParameter &_source);
};