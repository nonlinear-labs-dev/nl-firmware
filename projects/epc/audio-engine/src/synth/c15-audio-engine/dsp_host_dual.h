#pragma once

/******************************************************************************/
/** @file       dsp_host_dual.h
    @date
    @version    1.7-3
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

#include <ParameterMessages.h>
#include <PresetMessages.h>

#include "parameter_handle.h"
#include "pe_exponentiator.h"
#include "voice_allocation.h"

#include "ae_fadepoint.h"
#include "ae_global_section.h"
#include "ae_mono_section.h"
#include "ae_poly_section.h"

#include "midi_handle.h"

#include <array>
#include <Types.h>

// developer switches
inline constexpr unsigned int SAMPLE_COUNT = 0;  // 0: normal, 1: acceptance-test: does host really render?

// addendum - voice fade: enable proposed interpolated approach for generating fade values
inline constexpr bool VOICE_FADE_INTERPOLATION = true;

// basic logging switches
inline constexpr bool LOG_MISSING = false;
inline constexpr bool LOG_FAIL = false;
inline constexpr bool LOG_INIT = false;
inline constexpr bool LOG_MIDI_TCD = false;
inline constexpr bool LOG_MIDI_RAW = false;
inline constexpr bool LOG_MIDI_DETAIL = false;
inline constexpr bool LOG_MIDI_OUT = false;
inline constexpr bool LOG_DISPATCH = false;
inline constexpr bool LOG_EDITS = false;
inline constexpr bool LOG_TIMES = false;
inline constexpr bool LOG_SETTINGS = false;
inline constexpr bool LOG_RECALL = false;
inline constexpr bool LOG_RECALL_DETAILS = true;
inline constexpr bool LOG_RECALL_LEVELS = false;
inline constexpr bool LOG_KEYS = false;
inline constexpr bool LOG_KEYS_POLY = false;
inline constexpr bool LOG_TRANSITIONS = false;
inline constexpr bool LOG_RESET = false;
inline constexpr bool LOG_HW = false;
// more detailed logging of specific parameters
inline constexpr bool LOG_ENGINE_STATUS = false;
inline constexpr bool LOG_ENGINE_EDITS = false;
inline constexpr bool LOG_INPUT_EVENT_STAGE = false;
inline constexpr uint32_t LOG_PARAMS_LENGTH = 3;
// use tcd ids here (currently: Split Point, Unison Detune)
static const uint32_t LOG_PARAMS[LOG_PARAMS_LENGTH] = { 356, 250, 367 };

class MidiRuntimeOptions;

using KeyShift = ShifteableKeys<C15::Config::physical_key_from, C15::Config::physical_key_to>;

constexpr static auto tNUM_HW = static_cast<int>(C15::Parameters::Hardware_Sources::_LENGTH_);

class DSPInterface
{
 public:
  virtual ~DSPInterface() = default;
  //remove or move somewhere else
  //TCD and MIDI should not be known to DSP

  enum class InputEventSource
  {
    Internal,            //TCD -> use split
    External_Use_Split,  //Singular -> use split
    External_Primary,    //Primary Midi Channel
    External_Secondary,  //Secondary Midi Channel (UI: Split Channel)
    External_Both,       //Both   E.G Prim: CH1 + Sec: CH1
    Invalid
  };
  using OutputResetEventSource = AllocatorId;  // Reset Detection (return type for C15Synth)

  virtual void onHWChanged(HardwareSource id, float value, bool didBehaviourChange) = 0;
  virtual void onKeyDown(const int note, float velocity, InputEventSource from) = 0;
  virtual void onKeyUp(const int note, float velocity, InputEventSource from) = 0;
  virtual void onKeyDownSplit(const int note, float velocity, VoiceGroup part, InputEventSource from) = 0;
  virtual void onKeyUpSplit(const int note, float velocity, VoiceGroup part, InputEventSource from) = 0;
  virtual C15::Properties::HW_Return_Behavior getBehaviour(HardwareSource id) = 0;
  virtual SoundType getType() = 0;
  virtual VoiceGroup getSplitPartForKeyDown(int key) = 0;
  virtual VoiceGroup getSplitPartForKeyUp(int key, InputEventSource from) = 0;
  virtual VoiceGroup getNonLocalSplitKeyAssignmentForKeyUp(int key) = 0;
  virtual void registerNonLocalKeyAssignment(const int note, VoiceGroup part) = 0;
  virtual void unregisterNonLocalKeyAssignment(const int note) = 0;
  virtual void fadeOutResetVoiceAllocAndEnvelopes() = 0;
  virtual float getReturnValueFor(HardwareSource hwid)
  {
    return 0;
  }
  virtual void resetReturningHWSource(HardwareSource hwui)
  {
  }
  // areKeysPressed
  virtual bool areKeysPressed(SoundType _current)
  {
    return true;
  }
  // ...
  virtual void onSettingToneToggle(const uint16_t _setting)
  {
  }
  virtual OutputResetEventSource onSettingInitialSinglePreset()
  {
    return OutputResetEventSource::None;
  }
  //
  static inline uint32_t getInputSourceId(const InputEventSource _inputSource)
  {
    // InputEvent can be singular (TCD or Primary) or separate (Primary or Secondary or Both)
    // Secondary can exist, so the SourceId can be 0 (TCD), 1 (Primary) or 2 (Secondary) -- Both translates to Primary
    switch(_inputSource)
    {
      case InputEventSource::Internal:
        return 0;
      case InputEventSource::External_Use_Split:
      case InputEventSource::External_Primary:
      case InputEventSource::External_Both:
        return 1;
      case InputEventSource::External_Secondary:
        return 2;
    }
    nltools_assertAlways(false);
    return std::numeric_limits<uint32_t>::max();
  }

  void setHardwareSourceLastChangeSource(HardwareSource hw, HWChangeSource source)
  {
    if(hw != HardwareSource::NONE)
    {
      m_hwSourceLastChangeSources[static_cast<int>(hw)] = source;
    }
  }

  HWChangeSource getHardwareSourceLastChangeSource(HardwareSource hw)
  {
    if(hw != HardwareSource::NONE)
    {
      return m_hwSourceLastChangeSources[static_cast<int>(hw)];
    }

    nltools_assertNotReached();
    return HWChangeSource::TCD;
  }

 private:
  std::array<HWChangeSource, tNUM_HW> m_hwSourceLastChangeSources;
};

class dsp_host_dual : public DSPInterface
{
 public:
  void fadeOutResetVoiceAllocAndEnvelopes() override;
  // public members
  float m_mainOut_R = 0.0f, m_mainOut_L = 0.0f;
  uint32_t m_sample_counter = 0;
  // constructor
  dsp_host_dual();
  // public methods
  void init(const uint32_t _samplerate, const uint32_t _polyphony);
  // handles for inconvenient stuff
  C15::ParameterDescriptor getParameter(const int _id);
  // event bindings: debug
  void logStatus();
  // event bindings: Playcontroller or MIDI Device (in Dev_PC mode)

  using SimpleRawMidiMessage = nltools::msg::Midi::SimpleMessage;
  float getReturnValueFor(HardwareSource hwid) override;
  void resetReturningHWSource(HardwareSource hwui) override;
  bool areKeysPressed(SoundType _current) override;
  using MidiOut = std::function<void(const SimpleRawMidiMessage&)>;

  void onHWChanged(HardwareSource id, float value, bool didBehaviourChange) override;
  void onKeyDown(const int note, float velocity, InputEventSource from) override;
  void onKeyUp(const int note, float velocity, InputEventSource from) override;
  void onKeyDownSplit(const int note, float velocity, VoiceGroup part, InputEventSource from) override;
  void onKeyUpSplit(const int note, float velocity, VoiceGroup part, InputEventSource from) override;
  C15::Properties::HW_Return_Behavior getBehaviour(HardwareSource id) override;

  // event bindings: Preset Messages
  OutputResetEventSource onPresetMessage(const nltools::msg::SinglePresetMessage& _msg);
  OutputResetEventSource onPresetMessage(const nltools::msg::SplitPresetMessage& _msg);
  OutputResetEventSource onPresetMessage(const nltools::msg::LayerPresetMessage& _msg);

  // new ParameterChanged protocol
  void onParameterChangedMessage(const nltools::msg::HardwareAmountParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::MacroControlParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::MacroTimeParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::GlobalModulateableParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::GlobalUnmodulateableParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::LocalModulateableParameterChangedMessage& _msg);
  OutputResetEventSource
      onParameterChangedMessage(const nltools::msg::LocalUnmodulateableParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::PolyphonicModulateableParameterChangedMessage& _msg);
  OutputResetEventSource
      onParameterChangedMessage(const nltools::msg::PolyphonicUnmodulateableParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::MonophonicModulateableParameterChangedMessage& _msg);
  void onParameterChangedMessage(const nltools::msg::MonophonicUnmodulateableParameterChangedMessage& _msg);

  // todo: remove when unused
  void globalParChg(const uint32_t _id, const nltools::msg::HWAmountChangedMessage& _msg);
  void globalParChg(const uint32_t _id, const nltools::msg::MacroControlChangedMessage& _msg);
  void globalParChg(const uint32_t _id, const nltools::msg::ModulateableParameterChangedMessage& _msg);
  void globalParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  void globalTimeChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  void localParChg(const uint32_t _id, const nltools::msg::ModulateableParameterChangedMessage& _msg);
  void localParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  OutputResetEventSource localUnisonVoicesChg(const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  OutputResetEventSource localMonoEnableChg(const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  void localMonoPriorityChg(const nltools::msg::UnmodulateableParameterChangedMessage& _msg);
  void localMonoLegatoChg(const nltools::msg::UnmodulateableParameterChangedMessage& _msg);

  bool updateBehaviour(C15::ParameterDescriptor& param, ReturnMode mode);  // todo: refactor
  // evend bindings: Settings
  void onSettingEditTime(const float _position);
  void onSettingTransitionTime(const float _position);
  void onSettingGlitchSuppr(const bool _enabled);
  void onSettingTuneReference(const float _position);
  OutputResetEventSource onSettingInitialSinglePreset() override;
  void onSettingToneToggle(const uint16_t _setting) override;

  // dsp-related
  void render();
  void reset();

  using HWSourceValues = std::array<float, static_cast<size_t>(C15::Parameters::Hardware_Sources::_LENGTH_)>;
  HWSourceValues getHWSourceValues() const;
  SoundType getType() override;
  VoiceGroup getSplitPartForKeyDown(int key) override;
  VoiceGroup getSplitPartForKeyUp(int key, InputEventSource from) override;
  VoiceGroup getNonLocalSplitKeyAssignmentForKeyUp(int key) override;
  void registerNonLocalKeyAssignment(const int note, VoiceGroup part) override;
  void unregisterNonLocalKeyAssignment(const int note) override;

  using CC_Range_7_Bit = Midi::FullCCRange<Midi::Formats::_7_Bits_>;
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;
  using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;
  using CC_Range_Vel = Midi::clipped14BitVelRange;

 private:
  static inline VoiceGroup getVoiceGroupFromAllocatorId(const AllocatorId _id)
  {
    switch(_id)
    {
      case AllocatorId::Local_I:
        return VoiceGroup::I;
      case AllocatorId::Local_II:
        return VoiceGroup::II;
      case AllocatorId::Local_Both:
        return VoiceGroup::Global;
      default:
      case AllocatorId::None:
        return VoiceGroup::Invalid;
    }
  }
  using LayerMode = C15::Properties::LayerMode;
  static inline SoundType fromType(const LayerMode& _current)
  {
    switch(_current)
    {
      case LayerMode::Split:
        return SoundType::Split;
      case LayerMode::Layer:
        return SoundType::Layer;
    }
    return SoundType::Single;
  }
  // parameters (todo: migrate)
  Engine::Handle::ParameterHandle m_parameters;
  Engine::Param_Handle m_params;  // todo: remove
  Engine::Parameters::TimeParameter m_editTime, m_transitionTime;
  Time_Param m_edit_time, m_transition_time;  // todo: remove
  Setting_Param m_reference;
  const C15::ParameterDescriptor m_invalid_param = { C15::None };
  // essential tools
  exponentiator m_convert;
  Engine::Handle::Clock_Handle m_clock;
  Engine::Handle::Time_Handle m_time;
  // layer handling
  LayerMode m_layer_mode;
  // global dsp components
  Engine::GlobalSection m_global;
  VoiceAllocation<C15::Config::total_polyphony, C15::Config::local_polyphony, C15::Config::virtual_key_count,
                  C15::Config::generic_key_pivot, LayerMode>
      m_alloc;
  // dsp components
  atomic_fade_table m_fade;
  Engine::PolySection m_poly[2];
  Engine::MonoSection m_mono[2];
  Engine::LayerSignalCollection m_z_layers[2];
  // helper values
  const float m_format_vel = 16383.0f / 127.0f, m_format_hw = 16000.0f / 127.0f, m_format_pb = 16000.0f / 16383.0f,
              m_norm_vel = 1.0f / 16383.0f, m_norm_hw = 1.0f / 16000.0f;
  int32_t m_key_pos = 0;
  uint32_t m_tone_state = 0;
  bool m_glitch_suppression = false;

  inline void initLocalParameter(const C15::ParameterDescriptor& _desc);
  inline void initSmoothing(const C15::ParameterDescriptor& _desc);
  inline void initSmoothing(const uint32_t& _layer, const C15::ParameterDescriptor& _desc);
  // handles for inconvenient stuff
  C15::Properties::HW_Return_Behavior getBehavior(const ReturnMode _mode);
  C15::Properties::HW_Return_Behavior getBehavior(const RibbonReturnMode _mode);
  C15::Parameters::Macro_Controls getMacro(const MacroControls _mc);
  uint32_t getMacroId(const MacroControls _mc);
  C15::Properties::LayerId getLayer(const VoiceGroup _vg);
  uint32_t getLayerId(const VoiceGroup _vg);
  // key events
  void keyDownTraversal(const uint32_t _note, const float _vel, const uint32_t _inputSourceId);
  void keyUpTraversal(const uint32_t _note, const float _vel, const uint32_t _inputSourceId);
  float scale(const Engine::Parameters::Aspects::ScaleAspect::Scaling& _scl, float _value);
  float scale(const Scale_Aspect _scl, float _value);  // todo: remove
  void updateTime(Engine::Parameters::Aspects::TimeAspect::Time& _time, const float& _ms);
  void updateTime(Time_Aspect* _param, const float _ms);                      // todo: remove
  void hwModChain(HW_Src_Param* _src, const uint32_t _id, const float _inc);  // todo: refactor
  void globalModChain(Macro_Param* _mc);                                      // todo: remove
  void globalModChain(const Engine::Parameters::MacroControl& _mc);
  void localModChain(Macro_Param* _mc);                         // todo: remove
  void localModChain(const uint32_t _layer, Macro_Param* _mc);  // todo: remove
  void localModChain(const Engine::Parameters::MacroControl& _mc);
  void localModChain(const uint32_t& _layer, const Engine::Parameters::MacroControl& _mc);
  void globalTransition(const Target_Param* _param, const Time_Aspect _time);  // todo: remove
  void globalTransition(const Direct_Param* _param, const Time_Aspect _time);  // todo: remove
  template <typename T>
  inline void globalTransition(const T& _param, const Engine::Parameters::Aspects::TimeAspect::Time& _time);
  void localTransition(const uint32_t _layer, const Direct_Param* _param, const Time_Aspect _time);  // todo: remove
  void localTransition(const uint32_t _layer, const Target_Param* _param, const Time_Aspect _time);  // todo: remove
  template <typename T>
  inline void localTransition(const uint32_t& _layer, const T& _param,
                              const Engine::Parameters::Aspects::TimeAspect::Time& _time);

  // todo: https://github.com/nonlinear-labs-dev/C15/issues/2995
  template <typename T>
  inline void polyphonicTransition(const uint32_t& _layer, const T& _param,
                                   const Engine::Parameters::Aspects::TimeAspect::Time& _time);
  template <typename T>
  inline void monophonicTransition(const uint32_t& _layer, const T& _param,
                                   const Engine::Parameters::Aspects::TimeAspect::Time& _time);

  inline OutputResetEventSource onUnisonVoicesChanged(const uint32_t& _layer, const float& _pos);
  inline OutputResetEventSource onMonoEnableChanged(const uint32_t& _layer, const float& _pos);

  // todo: remove
  bool evalPolyChg(const C15::Properties::LayerId _layerId,
                   const nltools::msg::Parameters::UnmodulateableParameter& _unisonVoices,
                   const nltools::msg::Parameters::UnmodulateableParameter& _monoEnable);

  bool determinePolyChg(const C15::Properties::LayerId _layerId,
                        const nltools::controls::LocalUnmodulateableParameter& _unisonVoices,
                        const nltools::controls::LocalUnmodulateableParameter& _monoEnable);
  void evalVoiceFadeChg(const uint32_t _layer);
  OutputResetEventSource determineOutputEventSource(const bool _detected, const LayerMode _type);

  template<typename T>
  inline void recallCommon(const T& _msg, const bool _resetVoiceFade);
  template<typename T> inline void recallCommonTransition(const T& _msg);

  // todo: refactor
  OutputResetEventSource recallSingle(const nltools::msg::SinglePresetMessage& _msg);
  OutputResetEventSource recallSplit(const nltools::msg::SplitPresetMessage& _msg);
  OutputResetEventSource recallLayer(const nltools::msg::LayerPresetMessage& _msg);

  // todo: remove
  void globalParRcl(const nltools::msg::Parameters::HardwareSourceParameter& _param);
  void globalParRcl(const nltools::msg::Parameters::HardwareAmountParameter& _param);
  void globalParRcl(const nltools::msg::Parameters::MacroParameter& _param);
  void globalParRcl(const nltools::msg::Parameters::ModulateableParameter& _param);
  void globalParRcl(const nltools::msg::Parameters::UnmodulateableParameter& _param);
  void globalParRcl(const nltools::msg::Parameters::GlobalParameter& _param);
  void globalTimeRcl(const nltools::msg::Parameters::UnmodulateableParameter& _param);
  void localParRcl(const uint32_t _layerId, const nltools::msg::Parameters::ModulateableParameter& _param);
  void localParRcl(const uint32_t _layerId, const nltools::msg::Parameters::SplitPoint& _param);
  void localParRcl(const uint32_t _layerId, const nltools::msg::Parameters::UnmodulateableParameter& _param);
  void localPolyRcl(const uint32_t _layerId, const bool _va_update,
                    const nltools::msg::ParameterGroups::UnisonGroup& _unison,
                    const nltools::msg::ParameterGroups::MonoGroup& _mono);

  // new parameter recall protocol
  inline void onParameterRecall(const nltools::controls::HardwareSourceParameter& _param);
  inline void onParameterRecall(const nltools::controls::HardwareAmountParameter& _param);
  inline void onParameterRecall(const nltools::controls::MacroControlParameter& _param);
  inline void onParameterRecall(const nltools::controls::MacroTimeParameter& _param);
  inline void onParameterRecall(const nltools::controls::GlobalModulateableParameter& _param);
  inline void onParameterRecall(const nltools::controls::GlobalUnmodulateableParameter& _param);
  inline void onParameterRecall(const uint32_t& _layerId, const nltools::controls::LocalModulateableParameter& _param);
  inline void onParameterRecall(const uint32_t& _layerId, const nltools::controls::LocalUnmodulateableParameter& _param, const bool _vaUpdate);

  // todo: https://github.com/nonlinear-labs-dev/C15/issues/2995
  void onParameterRecall(const uint32_t& _layerId, const nltools::controls::PolyphonicModulateableParameter& _param);
  void onParameterRecall(const uint32_t& _layerId, const nltools::controls::PolyphonicUnmodulateableParameter& _param);
  void onParameterRecall(const uint32_t& _layerId, const nltools::controls::MonophonicModulateableParameter& _param);
  void onParameterRecall(const uint32_t& _layerId, const nltools::controls::MonophonicUnmodulateableParameter& _param);

  void debugLevels();

  friend class DspHostDualTester;
};
