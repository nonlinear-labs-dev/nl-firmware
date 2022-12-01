#include "dsp_host_dual.h"
#include <MidiRuntimeOptions.h>
#include <cassert>
#include <nltools/messaging/Messaging.h>
#include <parameter_group.h>

using namespace std::chrono_literals;

/******************************************************************************/
/** @file       dsp_host_dual.cpp
    @date
    @version    1.7-3
    @author     M. Seeber
    @brief      new main engine container.
    @todo
*******************************************************************************/

// shorthands for identifying parameters with special event logic
static constexpr auto IndexOfUnisonVoices = C15::ParameterList[C15::PID::Unison_Voices].m_param.m_index;
static constexpr auto IndexOfUnisonDetune = C15::ParameterList[C15::PID::Unison_Detune].m_param.m_index;
static constexpr auto IndexOfUnisonPhase = C15::ParameterList[C15::PID::Unison_Phase].m_param.m_index;
static constexpr auto IndexOfUnisonPan = C15::ParameterList[C15::PID::Unison_Pan].m_param.m_index;
static constexpr auto IndexOfMonoEnable = C15::ParameterList[C15::PID::Mono_Grp_Enable].m_param.m_index;
static constexpr auto IndexOfMonoGlide = C15::ParameterList[C15::PID::Mono_Grp_Glide].m_param.m_index;
static constexpr auto IndexOfVoiceGrpVolume = C15::ParameterList[C15::PID::Voice_Grp_Volume].m_param.m_index;
static constexpr auto IndexOfVoiceGrpTune = C15::ParameterList[C15::PID::Voice_Grp_Tune].m_param.m_index;
static constexpr auto IndexOfVoiceGrpMute = C15::ParameterList[C15::PID::Voice_Grp_Mute].m_param.m_index;
static constexpr auto IndexOfVoiceGrpFadeFrom = C15::ParameterList[C15::PID::Voice_Grp_Fade_From].m_param.m_index;
static constexpr auto IndexOfVoiceGrpFadeRange = C15::ParameterList[C15::PID::Voice_Grp_Fade_Range].m_param.m_index;
static constexpr auto IndexOfSplitPoint = C15::ParameterList[C15::PID::Split_Split_Point].m_param.m_index;
static constexpr auto IndexOfMixEffectsFrom = C15::ParameterList[C15::PID::FB_Mix_FX_Src].m_param.m_index;
static constexpr auto IndexOfMixToEffects = C15::ParameterList[C15::PID::Out_Mix_To_FX].m_param.m_index;
static constexpr auto IndexOfBaseKey = C15::ParameterList[C15::PID::Scale_Base_Key].m_param.m_index;
static constexpr auto IndexOfEffectsMix = C15::ParameterList[C15::PID::Master_FX_Mix].m_param.m_index;

dsp_host_dual::dsp_host_dual()
{
  m_mainOut_L = m_mainOut_R = 0.0f;
  m_layer_mode = C15::Properties::LayerMode::Single;
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{
  // BEWARE: we still only support TWO integer multiples of 48000 Hz ("SD": 1 x
  // 48k, "HD": 2 x 48k)
  const uint32_t upsampleFactor = (_samplerate > 96000 ? 96000 : _samplerate) / 48000,
                 upsampleIndex = upsampleFactor - 1;
  if(_samplerate != C15::Config::clock_rates[upsampleIndex][0])
  {
    nltools::Log::warning(__PRETTY_FUNCTION__, "invalid sample rate(", _samplerate,
                          "): only 48000 or 96000 Hz allowed!");
  }
  const float samplerate = static_cast<float>(C15::Config::clock_rates[upsampleIndex][0]);
  // init of crucial components: voiceAlloc, conversion, clock, time, fadepoint
  m_alloc.init();
  m_alloc.setSplitPoint(30 + C15::Config::physical_key_from, 0);  // temporary..?
  m_alloc.setSplitPoint(31 + C15::Config::physical_key_from, 1);  // temporary..?
  m_convert.init();
  m_clock.init(upsampleIndex);
  m_time.init(upsampleIndex);
  m_fade.init(samplerate);
  // proper time init
  m_editTime.init(C15::Properties::SmootherScale::Linear, 200.0f, 0.0f, 0.1f);
  m_editTime.m_scaled = scale(m_editTime.m_scaling, m_editTime.m_position);
  updateTime(m_editTime.m_time, m_editTime.m_scaled);
  m_transitionTime.init(C15::Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, 0.0f);
  m_transitionTime.m_scaled = scale(m_transitionTime.m_scaling, m_transitionTime.m_position);
  updateTime(m_transitionTime.m_time, m_transitionTime.m_scaled);
  // note: time and reference setting params are currently hard-coded but could
  // also be part of parameter list
  m_reference.init(C15::Properties::SmootherScale::Linear, 80.0f, 400.0f, 0.5f);
  m_reference.m_scaled = scale(m_reference.m_scaling, m_reference.m_position);
  // dsp sections init
  m_global.init(m_time.m_sample_inc);
  m_global.update_tone_amplitude(-6.0f);
  m_global.update_tone_frequency(m_reference.m_scaled);
  m_global.update_tone_mode(0);

  // voice fade stuff I (currently explicit)
  m_poly[0].m_fadeStart = C15::Config::virtual_key_from;
  m_poly[0].m_fadeEnd = C15::Config::virtual_key_to;
  m_poly[0].m_fadeIncrement = 1;
  // init poly dsp: exponentiator, feedback pointers
  m_poly[0].init(&m_global.m_signals, &m_convert, &m_time, &m_z_layers[0], &m_reference.m_scaled, m_time.m_millisecond,
                 env_init_gateRelease, samplerate, upsampleFactor);
  // voice fade stuff II (currently explicit)
  m_poly[1].m_fadeStart = C15::Config::virtual_key_to;
  m_poly[1].m_fadeEnd = C15::Config::virtual_key_from;
  m_poly[1].m_fadeIncrement = -1;
  // init poly dsp: exponentiator, feedback pointers
  m_poly[1].init(&m_global.m_signals, &m_convert, &m_time, &m_z_layers[1], &m_reference.m_scaled, m_time.m_millisecond,
                 env_init_gateRelease, samplerate, upsampleFactor);
  // init mono dsp
  m_mono[0].init(&m_convert, &m_z_layers[0], m_time.m_millisecond, samplerate, upsampleFactor);
  m_mono[1].init(&m_convert, &m_z_layers[1], m_time.m_millisecond, samplerate, upsampleFactor);
  // init parameters by parameter list
  m_parameters.init();

  for(const auto &element : C15::ParameterList)
  {
    using Type = C15::Descriptors::ParameterType;
    const uint32_t index = element.m_param.m_index;
    switch(element.m_param.m_type)
    {
      case Type::None:
      case Type::Hardware_Source:
      case Type::Display_Parameter:
      case Type::Hardware_Amount:
        break;
      case Type::Macro_Control:
        m_parameters.m_global.m_macroControls[index].init(element);
        break;
      case Type::Macro_Time:
      {
        auto &param = m_parameters.m_global.m_macroTimes[index];
        param.init(element);
        param.m_scaled = scale(param.m_scaling, param.m_position);
        updateTime(param.m_time, param.m_scaled);
        break;
      }
      case Type::Global_Modulateable:
      {
        auto &param = m_parameters.m_global.m_parameters.m_modulateables[index];
        param.init(element);
        initSmoothing(element);
        break;
      }
      case Type::Global_Unmodulateable:
      {
        auto &param = m_parameters.m_global.m_parameters.m_unmodulateables[index];
        param.init(element);
        initSmoothing(element);
        break;
      }
      default:
        initLocalParameter(element);
        break;
    }
  }

  // temporary: load initials in order to have valid osc reset params
  //onSettingInitialSinglePreset();

  if constexpr(LOG_INIT)
  {
    //nltools::Log::info("dsp_host_dual::init - engine dsp status: global");
    //nltools::Log::info("missing: nltools::msg - reference, initial:", m_reference.m_scaled);
  }
}

inline void dsp_host_dual::initLocalParameter(const C15::ParameterDescriptor &_desc)
{
  using Type = C15::Descriptors::ParameterType;
  const uint32_t index = _desc.m_param.m_index;
  for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
  {
    switch(_desc.m_param.m_type)
    {
      case Type::Polyphonic_Modulateable:
      {
        auto &param = m_parameters.m_layer[layer].m_polyphonic.m_modulateables[index];
        param.init(_desc);
        initSmoothing(layer, _desc);
        break;
      }
      case Type::Polyphonic_Unmodulateable:
      {
        auto &param = m_parameters.m_layer[layer].m_polyphonic.m_unmodulateables[index];
        param.init(_desc);
        initSmoothing(layer, _desc);
        break;
      }
      case Type::Monophonic_Modulateable:
      {
        auto &param = m_parameters.m_layer[layer].m_monophonic.m_modulateables[index];
        param.init(_desc);
        initSmoothing(layer, _desc);
        break;
      }
      case Type::Monophonic_Unmodulateable:
      {
        auto &param = m_parameters.m_layer[layer].m_monophonic.m_unmodulateables[index];
        param.init(_desc);
        initSmoothing(layer, _desc);
        break;
      }
      default:
        break;
    }
  }
}

inline void dsp_host_dual::initSmoothing(const C15::ParameterDescriptor &_desc)
{
  switch(_desc.m_ae.m_signal.m_signal)
  {
    case C15::Descriptors::ParameterSignal::Global_Signal:
      switch(_desc.m_ae.m_smoother.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_global.add_copy_sync_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_global.add_copy_audio_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_global.add_copy_fast_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_global.add_copy_slow_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
      }
      break;
    default:
      break;
  }
}

inline void dsp_host_dual::initSmoothing(const uint32_t &_layer, const C15::ParameterDescriptor &_desc)
{
  switch(_desc.m_ae.m_signal.m_signal)
  {
    case C15::Descriptors::ParameterSignal::Quasipoly_Signal:
      switch(_desc.m_ae.m_smoother.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_poly[_layer].add_copy_sync_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_poly[_layer].add_copy_audio_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_poly[_layer].add_copy_fast_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_poly[_layer].add_copy_slow_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
      }
      break;
    case C15::Descriptors::ParameterSignal::Mono_Signal:
      switch(_desc.m_ae.m_smoother.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_mono[_layer].add_copy_sync_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_mono[_layer].add_copy_audio_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_mono[_layer].add_copy_fast_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_mono[_layer].add_copy_slow_id(_desc.m_ae.m_smoother.m_index, _desc.m_ae.m_signal.m_index);
          break;
      }
      break;
    default:
      break;
  }
}

C15::ParameterDescriptor dsp_host_dual::getParameter(const int _id)
{
  if((_id > -1) && (_id < C15::Config::tcd_elements))
  {
    if constexpr(LOG_DISPATCH)
    {
      if(C15::ParameterList[_id].m_param.m_type != C15::Descriptors::ParameterType::None)
      {
        nltools::Log::info(__PRETTY_FUNCTION__, "(", _id,
                           "):", C15::ParameterGroups[(unsigned) C15::ParameterList[_id].m_group].m_label_short, "-",
                           C15::ParameterList[_id].m_pg.m_param_label_short);
      }
      else
      {
        nltools::Log::warning(__PRETTY_FUNCTION__, "dispatch(", _id, "): None!");
      }
    }
    return C15::ParameterList[_id];
  }
  if constexpr(LOG_FAIL)
  {
    nltools::Log::warning(__PRETTY_FUNCTION__, "dispatch(", _id, "):", "failed!");
  }
  return m_invalid_param;
}

void dsp_host_dual::logStatus()
{
  if constexpr(LOG_ENGINE_STATUS)
  {
    nltools::Log::info(__PRETTY_FUNCTION__);
    nltools::Log::info("-clock(index:", m_clock.m_index, ", fast:", m_clock.m_fast, ", slow:", m_clock.m_slow, ")");
    nltools::Log::info("-output(left:", m_mainOut_L, ", right:", m_mainOut_R, ", mute:", m_fade.getValue(), ")");
    nltools::Log::info("-dsp(dx:", m_time.m_sample_inc, ", ms:", m_time.m_millisecond, ")");
  }
  if constexpr(LOG_ENGINE_EDITS)
  {
    for(const auto id : LOG_PARAMETERS)
    {
      const auto &element = C15::ParameterList[(uint32_t) id];
      switch(element.m_param.m_type)
      {
        case C15::Descriptors::ParameterType::Hardware_Source:
          m_parameters.m_global.m_hardwareSources[element.m_param.m_index].log(__PRETTY_FUNCTION__, element);
          break;
        case C15::Descriptors::ParameterType::Hardware_Amount:
          m_parameters.m_global.m_hardwareAmounts[element.m_param.m_index].log(__PRETTY_FUNCTION__, element);
          break;
        case C15::Descriptors::ParameterType::Macro_Control:
          m_parameters.m_global.m_macroControls[element.m_param.m_index].log(__PRETTY_FUNCTION__, element);
          break;
        case C15::Descriptors::ParameterType::Macro_Time:
          m_parameters.m_global.m_macroTimes[element.m_param.m_index].log(__PRETTY_FUNCTION__, element);
          break;
        case C15::Descriptors::ParameterType::Global_Modulateable:
          m_parameters.m_global.m_parameters.m_modulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__, element);
          break;
        case C15::Descriptors::ParameterType::Global_Unmodulateable:
          m_parameters.m_global.m_parameters.m_unmodulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                            element);
          break;
        case C15::Descriptors::ParameterType::Polyphonic_Modulateable:
          m_parameters.m_layer[0].m_polyphonic.m_modulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                            element);
          m_parameters.m_layer[1].m_polyphonic.m_modulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                            element);
          break;
        case C15::Descriptors::ParameterType::Polyphonic_Unmodulateable:
          m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                              element);
          m_parameters.m_layer[1].m_polyphonic.m_unmodulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                              element);
          break;
        case C15::Descriptors::ParameterType::Monophonic_Modulateable:
          m_parameters.m_layer[0].m_monophonic.m_modulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                            element);
          m_parameters.m_layer[1].m_monophonic.m_modulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                            element);
          break;
        case C15::Descriptors::ParameterType::Monophonic_Unmodulateable:
          m_parameters.m_layer[0].m_monophonic.m_unmodulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                              element);
          m_parameters.m_layer[1].m_monophonic.m_unmodulateables[element.m_param.m_index].log(__PRETTY_FUNCTION__,
                                                                                              element);
          break;
        default:
          break;
      }
    }
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::onPresetMessage(const nltools::msg::SinglePresetMessage &_msg)
{
  if constexpr(LOG_RECALL)
  {
    // log preset with primitive timestamp (for debugging fade events)
    nltools::Log::info("Received Single Preset Message! (@", m_clock.m_index, ")");
  }

  if(m_glitch_suppression)
  {
    OutputResetEventSource result;
    // glitch suppression: start outputMute fade
    m_fade.muteAndDo(
        [&]
        {
          // global flush
          m_poly[0].flushDSP();
          m_poly[1].flushDSP();
          m_mono[0].flushDSP();
          m_mono[1].flushDSP();
          result = recallSingle(_msg);
        });

    if constexpr(LOG_RECALL)
    {
      nltools::Log::info(__PRETTY_FUNCTION__, m_fade.getNumMuteSamplesRendered(), "mute samples rendered.");
    }
    return result;  // return detected reset event
  }
  else
  {
    return recallSingle(_msg);  // return detected reset event
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::onPresetMessage(const nltools::msg::SplitPresetMessage &_msg)
{
  if constexpr(LOG_RECALL)
  {
    nltools::Log::info("Received Split Preset Message!, (@", m_clock.m_index, ")");
  }

  if(m_glitch_suppression)
  {
    OutputResetEventSource result;
    // glitch suppression: start outputMute fade
    m_fade.muteAndDo(
        [&]
        {
          // global flush
          m_poly[0].flushDSP();
          m_poly[1].flushDSP();
          m_mono[0].flushDSP();
          m_mono[1].flushDSP();
          result = recallSplit(_msg);
        });

    if constexpr(LOG_RECALL)
    {
      nltools::Log::info(__PRETTY_FUNCTION__, m_fade.getNumMuteSamplesRendered(), "mute samples rendered.");
    }
    return result;  // return detected reset event
  }
  else
  {
    return recallSplit(_msg);  // return detected reset event
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::onPresetMessage(const nltools::msg::LayerPresetMessage &_msg)
{
  if constexpr(LOG_RECALL)
  {
    nltools::Log::info("Received Layer Preset Message!, (@", m_clock.m_index, ")");
  }
  if(m_glitch_suppression)
  {
    OutputResetEventSource result;
    // glitch suppression: start outputMute fade
    m_fade.muteAndDo(
        [&]
        {
          // global flush
          m_poly[0].flushDSP();
          m_poly[1].flushDSP();
          m_mono[0].flushDSP();
          m_mono[1].flushDSP();
          result = recallLayer(_msg);
        });

    if constexpr(LOG_RECALL)
    {
      nltools::Log::info(__PRETTY_FUNCTION__, m_fade.getNumMuteSamplesRendered(), "mute samples rendered.");
    }
    return result;  // return detected reset event
  }
  else
  {
    return recallLayer(_msg);  // return detected reset event
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::HardwareAmountParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  auto &param = m_parameters.m_global.m_hardwareAmounts[descriptor.m_param.m_index];
  if(param.update_position((float) _msg.m_controlPosition))
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::MacroControlParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  auto &param = m_parameters.m_global.m_macroControls[descriptor.m_param.m_index];
  if(param.update_position((float) _msg.m_controlPosition))
  {
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
    param.m_unclipped = param.m_position;  // fixing #2023: unclipped always up-to-date
    mcModChain(param);
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::MacroTimeParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  auto &param = m_parameters.m_global.m_macroTimes[descriptor.m_param.m_index];
  if(param.update_position((float) _msg.m_controlPosition))
  {
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
    param.m_scaled = scale(param.m_scaling, param.m_position);
    updateTime(param.m_time, param.m_scaled);
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::GlobalModulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  const uint32_t index = descriptor.m_param.m_index, macroId = getMacroId(_msg.m_macro);
  auto &param = m_parameters.m_global.m_parameters.m_modulateables[index];
  bool aspect_update = param.update_source(getMacro(_msg.m_macro));
  if(aspect_update)
    m_parameters.m_global.m_parameters.m_assignment.reassign(index, macroId);
  aspect_update |= param.update_amount((float) _msg.m_modulationAmount);
  if(param.update_position(param.depolarize((float) _msg.m_controlPosition)))
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
    switch(_msg.m_id)
    {
      case C15::PID::Master_FX_Mix:
        if(m_layer_mode == LayerMode::Single)
          fxMixTransition(m_editTime.m_time, param.m_scaled);
        break;
      default:
        globalTransition(param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
    }
  }
  else if(aspect_update)
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::GlobalUnmodulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  auto &param = m_parameters.m_global.m_parameters.m_unmodulateables[descriptor.m_param.m_index];
  if(param.update_position((float) _msg.m_controlPosition))
  {
    param.m_scaled = scale(param.m_scaling, param.m_position);
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
    switch(_msg.m_id)
    {
      case C15::PID::Scale_Base_Key:
        m_global.start_base_key(m_editTime.m_time.m_dxSlow, param.m_scaled);
        break;
      default:
        globalTransition(param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
    }
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::PolyphonicModulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  const uint32_t index = descriptor.m_param.m_index, macroId = getMacroId(_msg.m_macro),
                 layerId = getLayerId(_msg.m_voiceGroup);
  auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_modulateables[index];
  bool aspect_update = param.update_source(getMacro(_msg.m_macro));
  if(aspect_update)
    m_parameters.m_layer[layerId].m_polyphonic.m_assignment.reassign(index, macroId);
  aspect_update |= param.update_amount((float) _msg.m_modulationAmount);
  if(param.update_position((float) param.depolarize(_msg.m_controlPosition)))
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
    if constexpr(LOG_EDITS)
      param.log(layerId, __PRETTY_FUNCTION__, descriptor);
    switch(_msg.m_id)
    {
      case C15::PID::Unison_Detune:
      case C15::PID::Unison_Phase:
      case C15::PID::Unison_Pan:
      case C15::PID::Mono_Grp_Glide:
        if(m_layer_mode != LayerMode::Split)
          for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
            localTransition(layer, param.m_rendering, m_editTime.m_time, param.m_scaled);
        else
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
      case C15::PID::Split_Split_Point:
        if(m_layer_mode == LayerMode::Split)
          m_alloc.setSplitPoint(static_cast<uint32_t>(param.m_scaled) + C15::Config::physical_key_from, layerId);
        break;
      case C15::PID::Voice_Grp_Volume:
      case C15::PID::Voice_Grp_Tune:
        // single: ignore, dual: pass
        if(m_layer_mode != LayerMode::Single)
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
      case C15::PID::FB_Mix_FX_Src:
      case C15::PID::Out_Mix_To_FX:
        if(m_layer_mode == LayerMode::Single)
        {
          // pass change to part I, invert change for part II
          localTransition(0, param.m_rendering, m_editTime.m_time, param.m_scaled);
          localTransition(1, param.m_rendering, m_editTime.m_time, 1.0f - param.m_scaled);
        }
        else
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
      default:
        if(m_layer_mode == LayerMode::Single)
          for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
            localTransition(layer, param.m_rendering, m_editTime.m_time, param.m_scaled);
        else
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
    }
  }
  else if(aspect_update)
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
  }
}

DSPInterface::OutputResetEventSource
    dsp_host_dual::onParameterChangedMessage(const nltools::msg::PolyphonicUnmodulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  const uint32_t index = descriptor.m_param.m_index, layerId = getLayerId(_msg.m_voiceGroup);
  auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[index];
  if(param.update_position((float) _msg.m_controlPosition))
  {
    param.m_scaled = scale(param.m_scaling, param.m_position);
    if constexpr(LOG_EDITS)
      param.log(layerId, __PRETTY_FUNCTION__, descriptor);
    switch(_msg.m_id)
    {
      case C15::PID::Voice_Grp_Mute:
        if(m_layer_mode == LayerMode::Layer)
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
      case C15::PID::Voice_Grp_Fade_From:
      case C15::PID::Voice_Grp_Fade_Range:
        if(m_layer_mode == LayerMode::Layer)
          evalVoiceFadeChg(layerId);
        break;
      case C15::PID::Unison_Voices:
        return onUnisonVoicesChanged(layerId, param.m_position);
      case C15::PID::Mono_Grp_Enable:
        return onMonoEnableChanged(layerId, param.m_scaled);
      case C15::PID::Mono_Grp_Prio:
        m_alloc.setMonoPriority(layerId, param.m_scaled, m_layer_mode);
        break;
      case C15::PID::Mono_Grp_Legato:
        m_alloc.setMonoLegato(layerId, param.m_scaled, m_layer_mode);
        break;
      default:
        if(m_layer_mode == LayerMode::Single)
          for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
            localTransition(layer, param.m_rendering, m_editTime.m_time, param.m_scaled);
        else
          localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
        break;
    }
  }
  return OutputResetEventSource::None;
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::MonophonicModulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  const uint32_t index = descriptor.m_param.m_index, macroId = getMacroId(_msg.m_macro),
                 layerId = getLayerId(_msg.m_voiceGroup);
  auto &param = m_parameters.m_layer[layerId].m_monophonic.m_modulateables[index];
  bool aspect_update = param.update_source(getMacro(_msg.m_macro));
  if(aspect_update)
    m_parameters.m_layer[layerId].m_monophonic.m_assignment.reassign(index, macroId);
  aspect_update |= param.update_amount((float) _msg.m_modulationAmount);
  if(param.update_position((float) param.depolarize(_msg.m_controlPosition)))
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
    if constexpr(LOG_EDITS)
      param.log(layerId, __PRETTY_FUNCTION__, descriptor);
    // no special cases, always part-bound
    localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
  }
  else if(aspect_update)
  {
    param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
    if constexpr(LOG_EDITS)
      param.log(__PRETTY_FUNCTION__, descriptor);
  }
}

void dsp_host_dual::onParameterChangedMessage(const nltools::msg::MonophonicUnmodulateableParameterChangedMessage &_msg)
{
  const auto &descriptor = getParameter(_msg.m_id);
  const uint32_t index = descriptor.m_param.m_index, layerId = getLayerId(_msg.m_voiceGroup);
  auto &param = m_parameters.m_layer[layerId].m_monophonic.m_unmodulateables[index];
  if(param.update_position((float) _msg.m_controlPosition))
  {
    param.m_scaled = scale(param.m_scaling, param.m_position);
    if constexpr(LOG_EDITS)
      param.log(layerId, __PRETTY_FUNCTION__, descriptor);
    // no special cases, always part-bound
    localTransition(layerId, param.m_rendering, m_editTime.m_time, param.m_scaled);
  }
}

inline DSPInterface::OutputResetEventSource dsp_host_dual::onUnisonVoicesChanged(const uint32_t &_layer,
                                                                                 const float &_pos)
{
  const OutputResetEventSource outputEvent
      = determineOutputEventSource(areKeysPressed(fromType(m_layer_mode)), m_layer_mode);
  // application now via fade point
  m_fade.muteAndDo(
      [&]
      {
        // apply (preloaded) unison change
        m_alloc.setUnison(_layer, _pos, m_layer_mode, m_layer_mode);
        // apply reset to affected poly compoments
        m_poly[_layer].resetEnvelopes();
        m_poly[_layer].m_uVoice = m_alloc.m_unison - 1;
        m_poly[_layer].m_key_active = 0;
        if(m_layer_mode != LayerMode::Split)
        {
          m_alloc.m_internal_keys.m_global = 0;
          // apply reset to other poly components (when not in split mode)
          const uint32_t layer = 1 - _layer;
          m_poly[layer].resetEnvelopes();
          m_poly[layer].m_uVoice = m_alloc.m_unison - 1;
          m_poly[layer].m_key_active = 0;
        }
        else
        {
          m_alloc.m_internal_keys.m_local[_layer] = 0;
        }
      });
  // return detected reset event
  if(m_layer_mode != LayerMode::Split)
  {
    return outputEvent;
  }
  else
  {
    const bool isPartI = _layer == 0;
    switch(outputEvent)
    {
      case OutputResetEventSource::Local_I:
        return isPartI ? outputEvent : OutputResetEventSource::None;
      case OutputResetEventSource::Local_II:
        return !isPartI ? outputEvent : OutputResetEventSource::None;
      case OutputResetEventSource::Local_Both:
        return isPartI ? OutputResetEventSource::Local_I : OutputResetEventSource::Local_II;
      default:
        return OutputResetEventSource::None;
    }
  }
}

inline DSPInterface::OutputResetEventSource dsp_host_dual::onMonoEnableChanged(const uint32_t &_layer,
                                                                               const float &_pos)
{
  const OutputResetEventSource outputEvent
      = determineOutputEventSource(areKeysPressed(fromType(m_layer_mode)), m_layer_mode);
  // application now via fade point
  m_fade.muteAndDo(
      [&]
      {
        // apply (preloaded) mono change
        m_alloc.setMonoEnable(_layer, _pos, m_layer_mode);
        // apply reset to affected poly compoments
        m_poly[_layer].resetEnvelopes();
        m_poly[_layer].m_key_active = 0;
        if(m_layer_mode != LayerMode::Split)
        {
          m_alloc.m_internal_keys.m_global = 0;
          // apply reset to other poly components (when not in split mode)
          const uint32_t lId = 1 - _layer;
          m_poly[lId].resetEnvelopes();
          m_poly[lId].m_key_active = 0;
        }
        else
        {
          m_alloc.m_internal_keys.m_local[_layer] = 0;
        }
      });
  // return detected reset event
  if(m_layer_mode != LayerMode::Split)
  {
    return outputEvent;
  }
  else
  {
    const bool isPartI = _layer == 0;
    switch(outputEvent)
    {
      case OutputResetEventSource::Local_I:
        return isPartI ? outputEvent : OutputResetEventSource::None;
      case OutputResetEventSource::Local_II:
        return !isPartI ? outputEvent : OutputResetEventSource::None;
      case OutputResetEventSource::Local_Both:
        return isPartI ? OutputResetEventSource::Local_I : OutputResetEventSource::Local_II;
      default:
        return OutputResetEventSource::None;
    }
  }
}

void dsp_host_dual::onSettingEditTime(const float _position)
{
  // inconvenient clamping of odd position range ...
  if(m_editTime.update_position(_position < 0.0f ? 0.0f : _position > 1.0f ? 1.0f : _position))
  {
    m_editTime.m_scaled = scale(m_editTime.m_scaling, m_editTime.m_position);
    updateTime(m_editTime.m_time, m_editTime.m_scaled);
    if constexpr(LOG_SETTINGS)
      nltools::Log::info(__PRETTY_FUNCTION__, "(position:", m_editTime.m_position, ", ms:", m_editTime.m_scaled, ")");
  }
}

void dsp_host_dual::onSettingTransitionTime(const float _position)
{
  // inconvenient clamping of odd position range ...
  if(m_transitionTime.update_position(_position < 0.0f ? 0.0f : _position > 1.0f ? 1.0f : _position))
  {
    m_transitionTime.m_scaled = scale(m_transitionTime.m_scaling, m_transitionTime.m_position);
    updateTime(m_transitionTime.m_time, m_transitionTime.m_scaled);
    if constexpr(LOG_SETTINGS)
      nltools::Log::info(__PRETTY_FUNCTION__, "(position:", m_transitionTime.m_position,
                         ", ms:", m_transitionTime.m_scaled, ")");
  }
}

void dsp_host_dual::onSettingGlitchSuppr(const bool _enabled)
{
  m_glitch_suppression = _enabled;
  if constexpr(LOG_SETTINGS)
    nltools::Log::info("glitch_suppression:", _enabled);
}

void dsp_host_dual::onSettingTuneReference(const float _position)
{
  // inconvenient clamping of odd position range ...
  if(m_reference.update_position(_position < 0.0f ? 0.0f : _position > 1.0f ? 1.0f : _position))
  {
    m_reference.m_scaled = scale(m_reference.m_scaling, m_reference.m_position);
    m_global.update_tone_frequency(m_reference.m_scaled);
    if constexpr(LOG_SETTINGS)
      nltools::Log::info(__PRETTY_FUNCTION__, "(position:", m_reference.m_position, ", ms:", m_reference.m_scaled, ")");
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::onSettingInitialSinglePreset()
{
  auto msg = nltools::msg::SinglePresetMessage {};
  for(const auto &element : C15::ParameterList)
  {
    const auto index = element.m_param.m_index;
    switch(element.m_param.m_type)
    {
      case C15::Descriptors::ParameterType::Hardware_Source:
        msg.m_hardwareSources[index].m_id = element.m_param_id;
        msg.m_hardwareSources[index].m_controlPosition = (double) element.m_initial;
        switch(element.m_param_id)
        {
          case C15::PID::Bender:
            msg.m_hardwareSources[index].m_returnMode = ReturnMode::Center;
            break;
          case C15::PID::Aftertouch:
            msg.m_hardwareSources[index].m_returnMode = ReturnMode::Zero;
            break;
          default:
            msg.m_hardwareSources[index].m_returnMode = ReturnMode::None;
            break;
        }
        break;
      case C15::Descriptors::ParameterType::Hardware_Amount:
        msg.m_hardwareAmounts[index].m_id = element.m_param_id;
        msg.m_hardwareAmounts[index].m_controlPosition = (double) element.m_initial;
        break;
      case C15::Descriptors::ParameterType::Macro_Control:
        msg.m_macroControls[index].m_id = element.m_param_id;
        msg.m_macroControls[index].m_controlPosition = (double) element.m_initial;
        break;
      case C15::Descriptors::ParameterType::Macro_Time:
        msg.m_macroTimes[index].m_id = element.m_param_id;
        msg.m_macroTimes[index].m_controlPosition = (double) element.m_initial;
        break;
      case C15::Descriptors::ParameterType::Global_Modulateable:
        msg.m_globalModulateables[index].m_id = element.m_param_id;
        msg.m_globalModulateables[index].m_controlPosition = (double) element.m_initial;
        msg.m_globalModulateables[index].m_macro = MacroControls::NONE;
        msg.m_globalModulateables[index].m_modulationAmount = 0.0;
        break;
      case C15::Descriptors::ParameterType::Global_Unmodulateable:
        msg.m_globalUnmodulateables[index].m_id = element.m_param_id;
        msg.m_globalUnmodulateables[index].m_controlPosition = (double) element.m_initial;
        break;
      case C15::Descriptors::ParameterType::Polyphonic_Modulateable:
        msg.m_polyphonicModulateables[index].m_id = element.m_param_id;
        msg.m_polyphonicModulateables[index].m_controlPosition
            = element.m_param_id == C15::PID::Out_Mix_A_Lvl ? 0.5 : (double) element.m_initial;
        msg.m_polyphonicModulateables[index].m_macro = MacroControls::NONE;
        msg.m_polyphonicModulateables[index].m_modulationAmount = 0.0;
        break;
      case C15::Descriptors::ParameterType::Polyphonic_Unmodulateable:
        msg.m_polyphonicUnmodulateables[index].m_id = element.m_param_id;
        msg.m_polyphonicUnmodulateables[index].m_controlPosition = (double) element.m_initial;
        break;
      case C15::Descriptors::ParameterType::Monophonic_Modulateable:
        for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
        {
          msg.m_monophonicModulateables[layer][index].m_id = element.m_param_id;
          msg.m_monophonicModulateables[layer][index].m_controlPosition = (double) element.m_initial;
          msg.m_monophonicModulateables[layer][index].m_macro = MacroControls::NONE;
          msg.m_monophonicModulateables[layer][index].m_modulationAmount = 0.0;
        }
        break;
      case C15::Descriptors::ParameterType::Monophonic_Unmodulateable:
        for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
        {
          msg.m_monophonicUnmodulateables[layer][index].m_id = element.m_param_id;
          msg.m_monophonicUnmodulateables[layer][index].m_controlPosition = (double) element.m_initial;
        }
        break;
      default:
        break;
    }
  }
  return onPresetMessage(msg);
}

void dsp_host_dual::onSettingToneToggle(const uint16_t _setting)
{
  m_tone_state = (_setting == 0 ? m_tone_state + 1 : _setting - 1) % 3;
  m_fade.muteAndDo([&] { m_global.update_tone_mode(m_tone_state); });
  // this is a crucial developer tool that should always produce logs!
  switch(m_tone_state)
  {
    case 0:
      nltools::Log::info("test tone - off (synth only)");
      break;
    case 1:
      nltools::Log::info("test tone - only (synth off)");
      break;
    case 2:
      nltools::Log::info("test tone - on (synth on)");
      break;
  }
}

void dsp_host_dual::render()
{
  m_sample_counter += SAMPLE_COUNT;
  m_fade.evalTaskStatus();

  if(m_fade.isMuted())
  {
    m_mainOut_L = m_mainOut_R = 0.0f;
    m_fade.onMuteSampleRendered();
    return;
  }

  m_clock.render();

  // slow rendering
  if(m_clock.m_slow)
  {
    // render components
    m_global.render_slow();
    m_poly[0].render_slow();
    m_poly[1].render_slow();
    m_mono[0].render_slow();
    m_mono[1].render_slow();
  }
  // fast rendering
  if(m_clock.m_fast)
  {
    // render components
    m_global.render_fast();
    m_poly[0].render_fast();
    m_poly[1].render_fast();
    m_mono[0].render_fast();
    m_mono[1].render_fast();
  }
  // audio rendering (always) -- temporary soundgenerator patching
  const float mute = m_fade.getValue();
  // - audio dsp poly - first stage - both layers (up to Output Mixer)
  m_poly[0].render_audio(mute);
  m_poly[1].render_audio(mute);
  // - audio dsp mono - each layer with separate sends - left, right), #2996: fx
  // cross-feedback
  const float left[2] = { m_poly[0].m_send_self_l + m_poly[1].m_send_other_l + m_z_layers[1].m_fx_to_other_l,
                          m_poly[0].m_send_other_l + m_poly[1].m_send_self_l + m_z_layers[0].m_fx_to_other_l };
  const float right[2] = { m_poly[0].m_send_self_r + m_poly[1].m_send_other_r + m_z_layers[1].m_fx_to_other_r,
                           m_poly[0].m_send_other_r + m_poly[1].m_send_self_r + m_z_layers[0].m_fx_to_other_r };
  m_mono[0].render_audio(left[0], right[0], m_poly[0].getVoiceGroupVolume(),
                         m_global.m_signals.get(C15::Signals::Global_Signals::Master_FX_I_to_II));
  m_mono[1].render_audio(left[1], right[1], m_poly[1].getVoiceGroupVolume(),
                         m_global.m_signals.get(C15::Signals::Global_Signals::Master_FX_II_to_I));
  // audio dsp poly - second stage - both layers (FB Mixer)
  m_poly[0].render_feedback(m_z_layers[1]);  // pass other layer's signals as arg
  m_poly[1].render_feedback(m_z_layers[0]);  // pass other layer's signals as arg
  // - audio dsp global - main out: combine layers, apply test_tone and soft
  // clip
  m_global.render_audio(m_mono[0].m_out_l, m_mono[0].m_out_r, m_mono[1].m_out_l, m_mono[1].m_out_r);
  // - final: main out, output mute
  m_mainOut_L = m_global.m_out_l * mute;
  m_mainOut_R = m_global.m_out_r * mute;
}

void dsp_host_dual::reset()
{
  m_fade.muteAndDo(
      [&]
      {
        for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
        {
          m_z_layers[layerId].reset();
          m_poly[layerId].resetDSP();
          m_mono[layerId].resetDSP();
        }
        m_alloc.reset();
        m_global.resetDSP();
        m_mainOut_L = m_mainOut_R = 0.0f;
        if constexpr(LOG_RESET)
        {
          nltools::Log::info("DSP has been reset.");
        }
      });
}

dsp_host_dual::HWSourceValues dsp_host_dual::getHWSourceValues() const
{
  dsp_host_dual::HWSourceValues ret;
  std::transform(m_parameters.m_global.m_hardwareSources.begin(), m_parameters.m_global.m_hardwareSources.end(),
                 ret.begin(), [](const auto &a) { return a.m_position; });
  return ret;
}

C15::Properties::HW_Return_Behavior dsp_host_dual::getBehavior(const ReturnMode _mode)
{
  switch(_mode)
  {
    case ReturnMode::None:
      return C15::Properties::HW_Return_Behavior::Stay;
    case ReturnMode::Zero:
      return C15::Properties::HW_Return_Behavior::Zero;
    case ReturnMode::Center:
      return C15::Properties::HW_Return_Behavior::Center;
    default:
      return C15::Properties::HW_Return_Behavior::Stay;
  }
}

C15::Properties::HW_Return_Behavior dsp_host_dual::getBehavior(const RibbonReturnMode _mode)
{
  switch(_mode)
  {
    case RibbonReturnMode::STAY:
      return C15::Properties::HW_Return_Behavior::Stay;
    case RibbonReturnMode::RETURN:
      return C15::Properties::HW_Return_Behavior::Center;
    default:
      return C15::Properties::HW_Return_Behavior::Stay;
  }
}

C15::Parameters::Macro_Controls dsp_host_dual::getMacro(const MacroControls _mc)
{
  switch(_mc)
  {
    case MacroControls::MC1:
      return C15::Parameters::Macro_Controls::MC_A;
    case MacroControls::MC2:
      return C15::Parameters::Macro_Controls::MC_B;
    case MacroControls::MC3:
      return C15::Parameters::Macro_Controls::MC_C;
    case MacroControls::MC4:
      return C15::Parameters::Macro_Controls::MC_D;
    case MacroControls::MC5:
      return C15::Parameters::Macro_Controls::MC_E;
    case MacroControls::MC6:
      return C15::Parameters::Macro_Controls::MC_F;
    default:
      return C15::Parameters::Macro_Controls::None;
  }
}

uint32_t dsp_host_dual::getMacroId(const MacroControls _mc)
{
  return static_cast<uint32_t>(getMacro(_mc));
}

C15::Properties::LayerId dsp_host_dual::getLayer(const VoiceGroup _vg)
{
  switch(_vg)
  {
    case VoiceGroup::I:
      return C15::Properties::LayerId::I;
    case VoiceGroup::II:
      return C15::Properties::LayerId::II;
    default:
      return C15::Properties::LayerId::I;
  }
}

uint32_t dsp_host_dual::getLayerId(const VoiceGroup _vg)
{
  switch(_vg)
  {
    case VoiceGroup::I:
      return 0;
    case VoiceGroup::II:
      return 1;
    default:
      return 0;
  }
}

void dsp_host_dual::keyDownTraversal(const uint32_t _note, const float _vel, const uint32_t _inputSourceId)
{
  if constexpr(LOG_KEYS)
  {
    nltools::Log::info("key_down(src:", _inputSourceId, ", pos:", _note, ", vel:", _vel, ", unison:", m_alloc.m_unison,
                       ")");
  }
  // prepares elevation, retrigger hardness
  // (would be nice to only execute in relevant part(s))
  m_poly[0].onBeforeKeyDown();
  m_poly[1].onBeforeKeyDown();
  for(auto event = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); event = m_alloc.m_traversal.next())
  {
    if(m_poly[event->m_localIndex].keyDown(event))
    {
      // mono legato
      m_mono[event->m_localIndex].keyDown(event);
    }
    if constexpr(LOG_KEYS_POLY)
    {
      nltools::Log::info("key_down_poly(group:", event->m_localIndex, "voice:", event->m_voiceId,
                         ", unisonIndex:", event->m_unisonIndex, ", stolen:", event->m_stolen, ", tune:", event->m_tune,
                         ", velocity:", event->m_velocity, ")");
      nltools::Log::info("key_details(active:", event->m_active, ", trigger_env:", event->m_trigger_env,
                         ", trigger_glide:", event->m_trigger_glide, ")");
    }
  }
}

void dsp_host_dual::keyUpTraversal(const uint32_t _note, const float _vel, const uint32_t _inputSourceId)
{
  if constexpr(LOG_KEYS)
  {
    nltools::Log::info("key_up(src:", _inputSourceId, ", pos:", _note, ", vel:", _vel, ", unison:", m_alloc.m_unison,
                       ")");
  }
  for(auto event = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); event = m_alloc.m_traversal.next())
  {
    m_poly[event->m_localIndex].keyUp(event);
    if constexpr(LOG_KEYS_POLY)
    {
      nltools::Log::info("key_up_poly(group:", event->m_localIndex, "voice:", event->m_voiceId,
                         ", tune:", event->m_tune, ", velocity:", event->m_velocity, ")");
      nltools::Log::info("key_details(active:", event->m_active, ", trigger_env:", event->m_trigger_env,
                         ", trigger_glide:", event->m_trigger_glide, ")");
    }
  }
}

float dsp_host_dual::scale(const Engine::Parameters::Aspects::ScaleAspect::Scaling &_scl, float _value)
{
  float result = 0.0f;
  switch(_scl.m_scaleId)
  {
    case C15::Properties::SmootherScale::None:
      break;
    case C15::Properties::SmootherScale::Linear:
      result = _scl.m_scaleOffset + (_scl.m_scaleFactor * _value);
      break;
    case C15::Properties::SmootherScale::Linear2Sections:
    {
      constexpr auto lim = 10.0f / 21.0f;
      constexpr auto normalizedInner = 1.0f / lim;
      constexpr auto normalizedOuter = 1.0f / (1.0f - lim);
      if((_value > -lim) && _value < lim)
      {
        result = _value * normalizedInner;
      }
      else if(_value >= lim)
      {
        result = 1.0f + (11.0f * (_value - lim) * normalizedOuter);
      }
      else
      {
        result = -1.0f + (11.0f * (_value + lim) * normalizedOuter);
      }
      break;
    }
    case C15::Properties::SmootherScale::Parabolic:
      result = _scl.m_scaleOffset + (_scl.m_scaleFactor * _value * std::abs(_value));
      break;
    case C15::Properties::SmootherScale::Cubic:
      result = _scl.m_scaleOffset + (_scl.m_scaleFactor * _value * _value * _value);
      break;
    case C15::Properties::SmootherScale::S_Curve:
      _value = (2.0f * (1.0f - _value)) - 1.0f;
      result
          = _scl.m_scaleOffset + (_scl.m_scaleFactor * ((_value * _value * _value * -0.25f) + (_value * 0.75f) + 0.5f));
      break;
    case C15::Properties::SmootherScale::Expon_Gain:
      result = m_convert.eval_level(_scl.m_scaleOffset + (_scl.m_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Osc_Pitch:
      result = m_convert.eval_osc_pitch(_scl.m_scaleOffset + (_scl.m_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Lin_Pitch:
      result = m_convert.eval_lin_pitch(_scl.m_scaleOffset + (_scl.m_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Shaper_Drive:
      result = (m_convert.eval_level(_value * _scl.m_scaleFactor) * _scl.m_scaleOffset) - _scl.m_scaleOffset;
      break;
    case C15::Properties::SmootherScale::Expon_Mix_Drive:
      result = _scl.m_scaleOffset * m_convert.eval_level(_scl.m_scaleFactor * _value);
      break;
    case C15::Properties::SmootherScale::Expon_Env_Time:
      result = m_convert.eval_time((_value * _scl.m_scaleFactor * 104.0781f) + _scl.m_scaleOffset);
      break;
  }
  return result;
}

void dsp_host_dual::updateTime(Engine::Parameters::Aspects::TimeAspect::Time &_time, const float &_ms)
{
  m_time.update_ms(_ms);
  _time.m_dxAudio = m_time.m_dx_audio;
  _time.m_dxFast = m_time.m_dx_fast;
  _time.m_dxSlow = m_time.m_dx_slow;
  if constexpr(LOG_TIMES)
  {
    nltools::Log::info(__PRETTY_FUNCTION__, "(ms: ", _ms, ", dx: [", m_time.m_dx_audio, ", ", m_time.m_dx_fast, ", ",
                       m_time.m_dx_slow, "])");
  }
}

void dsp_host_dual::hwModChain(const Engine::Parameters::HardwareSource &_src, const uint32_t &_id, const float &_inc)
{
  if constexpr(LOG_HW)
    nltools::Log::info(__PRETTY_FUNCTION__, "(id:", _id, ", pos:", _src.m_position, ")");
  if(_src.m_behavior == C15::Properties::HW_Return_Behavior::Stay)
  {
    for(uint32_t macroId = 0; macroId < C15::Parameters::num_of_Macro_Controls; macroId++)
    {
      const uint32_t amountIndex = _src.m_offset + macroId;
      const auto &amount = m_parameters.m_global.m_hardwareAmounts[amountIndex];
      if(amount.m_position != 0.0f)
      {
        auto &macro = m_parameters.m_global.m_macroControls[macroId];
        macro.m_unclipped = macro.m_position = _src.m_position;  // fixing #2023: unclipped always up-to-date
        mcModChain(macro);
      }
    }
  }
  else
  {
    for(uint32_t macroId = 0; macroId < C15::Parameters::num_of_Macro_Controls; macroId++)
    {
      const uint32_t amountIndex = _src.m_offset + macroId;
      const auto &amount = m_parameters.m_global.m_hardwareAmounts[amountIndex];
      if(amount.m_position != 0.0f)
      {
        auto &macro = m_parameters.m_global.m_macroControls[macroId];
        // fixing #2023:
        // only rely on unclipped (always up-to-date)
        // NOTE: hopefully, this won't introduce accumulating floating point
        // rounding errors !!!
        macro.m_unclipped += _inc * amount.m_position;
        const float clipped = std::clamp(macro.m_unclipped, 0.0f, 1.0f);
        if(macro.m_position != clipped)
        {
          macro.m_position = clipped;
          mcModChain(macro);
        }
      }
    }
  }
}

void dsp_host_dual::mcModChain(const Engine::Parameters::MacroControl &_mc)
{
  globalModChain(_mc);
  if(m_layer_mode == LayerMode::Single)
    polyphonicModChain(_mc);
  else
    for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
      polyphonicModChain(layerId, _mc);
  monophonicModChain(_mc);
}

inline void dsp_host_dual::globalModChain(const Engine::Parameters::MacroControl &_mc)
{
  auto &assignment = m_parameters.m_global.m_parameters.m_assignment;
  const auto &time = m_parameters.m_global.m_macroTimes[_mc.m_index].m_time;
  for(auto index = assignment.first(_mc.m_index); assignment.running(); index = assignment.next())
  {
    auto &param = m_parameters.m_global.m_parameters.m_modulateables[index];
    if(param.modulate(_mc.m_position))
    {
      const float clipped = param.m_unclipped < 0.0f ? 0.0f : param.m_unclipped > 1.0f ? 1.0f : param.m_unclipped;
      if(param.m_position != clipped)
      {
        param.m_position = clipped;
        param.m_scaled = scale(param.m_scaling, param.polarize(clipped));
        if constexpr(LOG_MOD_CHAIN)
          nltools::Log::info(__PRETTY_FUNCTION__, "(index:", index, ", pos:", param.m_position, ")");
        // todo: Fx Mix
        switch(index)
        {
          case IndexOfEffectsMix:
            fxMixTransition(time, param.m_scaled);
            break;
          default:
            globalTransition(param.m_rendering, time, param.m_scaled);
            break;
        }
      }
    }
  }
}

inline void dsp_host_dual::polyphonicModChain(const Engine::Parameters::MacroControl &_mc)
{
  auto &assignment = m_parameters.m_layer[0].m_polyphonic.m_assignment;
  const auto &time = m_parameters.m_global.m_macroTimes[_mc.m_index].m_time;
  for(auto index = assignment.first(_mc.m_index); assignment.running(); index = assignment.next())
  {
    auto &param = m_parameters.m_layer[0].m_polyphonic.m_modulateables[index];
    if(param.modulate(_mc.m_position))
    {
      const float clipped = param.m_unclipped < 0.0f ? 0.0f : param.m_unclipped > 1.0f ? 1.0f : param.m_unclipped;
      if(param.m_position != clipped)
      {
        param.m_position = clipped;
        param.m_scaled = scale(param.m_scaling, param.polarize(clipped));
        if constexpr(LOG_MOD_CHAIN)
          nltools::Log::info(__PRETTY_FUNCTION__, "(index:", index, ", pos:", param.m_position, ")");
        switch(index)
        {
          case IndexOfMixEffectsFrom:
          case IndexOfMixToEffects:
            localTransition(0, param.m_rendering, time, param.m_scaled);
            localTransition(1, param.m_rendering, time, 1.0f - param.m_scaled);
            break;
          default:
            for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
              localTransition(layer, param.m_rendering, time, param.m_scaled);
            break;
        }
      }
    }
  }
}

inline void dsp_host_dual::polyphonicModChain(const uint32_t &_layer, const Engine::Parameters::MacroControl &_mc)
{
  auto &assignment = m_parameters.m_layer[_layer].m_polyphonic.m_assignment;
  const auto &time = m_parameters.m_global.m_macroTimes[_mc.m_index].m_time;
  for(auto index = assignment.first(_mc.m_index); assignment.running(); index = assignment.next())
  {
    auto &param = m_parameters.m_layer[_layer].m_polyphonic.m_modulateables[index];
    if(param.modulate(_mc.m_position))
    {
      const float clipped = param.m_unclipped < 0.0f ? 0.0f : param.m_unclipped > 1.0f ? 1.0f : param.m_unclipped;
      if(param.m_position != clipped)
      {
        param.m_position = clipped;
        param.m_scaled = scale(param.m_scaling, param.polarize(clipped));
        if constexpr(LOG_MOD_CHAIN)
          nltools::Log::info(__PRETTY_FUNCTION__, "(layer:", _layer, ", index:", index, ", pos:", param.m_position,
                             ")");
        switch(index)
        {
          case IndexOfSplitPoint:
            m_alloc.setSplitPoint(static_cast<uint32_t>(param.m_scaled) + C15::Config::physical_key_from, _layer);
            break;
          default:
            localTransition(_layer, param.m_rendering, time, param.m_scaled);
            break;
        }
      }
    }
  }
}

inline void dsp_host_dual::monophonicModChain(const Engine::Parameters::MacroControl &_mc)
{
  for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
  {
    auto &assignment = m_parameters.m_layer[layer].m_monophonic.m_assignment;
    const auto &time = m_parameters.m_global.m_macroTimes[_mc.m_index].m_time;
    for(auto index = assignment.first(_mc.m_index); assignment.running(); index = assignment.next())
    {
      auto &param = m_parameters.m_layer[layer].m_monophonic.m_modulateables[index];
      if(param.modulate(_mc.m_position))
      {
        const float clipped = param.m_unclipped < 0.0f ? 0.0f : param.m_unclipped > 1.0f ? 1.0f : param.m_unclipped;
        if(param.m_position != clipped)
        {
          param.m_position = clipped;
          param.m_scaled = scale(param.m_scaling, param.polarize(clipped));
          if constexpr(LOG_MOD_CHAIN)
            nltools::Log::info(__PRETTY_FUNCTION__, "(layer:", layer, ", index:", index, ", pos:", param.m_position,
                               ")");
          localTransition(layer, param.m_rendering, time, param.m_scaled);
        }
      }
    }
  }
}

inline void dsp_host_dual::fxMixTransition(const Engine::Parameters::Aspects::TimeAspect::Time &_time,
                                           const float &_mix)
{
  const float invertedMix = 1.0f - _mix;
  const float partVolumes[C15::Properties::num_of_VoiceGroups]
      = { 1.0f - (_mix * _mix), 1.0f - (invertedMix * invertedMix) };
  for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
    localTransition(layer, m_parameters.m_layer[layer].m_polyphonic.m_modulateables[IndexOfVoiceGrpVolume].m_rendering,
                    _time, partVolumes[layer]);
}

inline void dsp_host_dual::globalTransition(const Engine::Parameters::Aspects::RenderAspect::Rendering &_rendering,
                                            const Engine::Parameters::Aspects::TimeAspect::Time &_time,
                                            const float &_dest)
{
  if(_rendering.m_renderSection == C15::Descriptors::SmootherSection::Global)
  {
    switch(_rendering.m_renderClock)
    {
      case C15::Descriptors::SmootherClock::Sync:
        m_global.start_sync(_rendering.m_renderIndex, _dest);
        break;
      case C15::Descriptors::SmootherClock::Audio:
        m_global.start_audio(_rendering.m_renderIndex, _time.m_dxAudio, _dest);
        break;
      case C15::Descriptors::SmootherClock::Fast:
        m_global.start_fast(_rendering.m_renderIndex, _time.m_dxFast, _dest);
        break;
      case C15::Descriptors::SmootherClock::Slow:
        m_global.start_slow(_rendering.m_renderIndex, _time.m_dxSlow, _dest);
        break;
    }
    if constexpr(LOG_TRANSITIONS)
      nltools::Log::info(__PRETTY_FUNCTION__, "global (index:", _rendering.m_renderIndex,
                         ", clock:", (int) _rendering.m_renderClock, ", dest:", _dest, ")");
  }
  else if constexpr(LOG_FAIL)
    nltools::Log::warning(__PRETTY_FUNCTION__, "(section:", (int) _rendering.m_renderSection,
                          ", clock:", (int) _rendering.m_renderClock, ", index:", _rendering.m_renderIndex, ")");
}

inline void dsp_host_dual::localTransition(const uint32_t &_layer,
                                           const Engine::Parameters::Aspects::RenderAspect::Rendering &_rendering,
                                           const Engine::Parameters::Aspects::TimeAspect::Time &_time,
                                           const float &_dest)
{
  switch(_rendering.m_renderSection)
  {
    case C15::Descriptors::SmootherSection::Poly:
      polyphonicTransition(_layer, _rendering, _time, _dest);
      break;
    case C15::Descriptors::SmootherSection::Mono:
      monophonicTransition(_layer, _rendering, _time, _dest);
      break;
    default:
      if constexpr(LOG_FAIL)
        nltools::Log::warning(__PRETTY_FUNCTION__, "(layer:", _layer, "section:", (int) _rendering.m_renderSection,
                              ", clock:", (int) _rendering.m_renderClock, ", index:", _rendering.m_renderIndex, ")");
      break;
  }
}

inline void dsp_host_dual::polyphonicTransition(const uint32_t &_layer,
                                                const Engine::Parameters::Aspects::RenderAspect::Rendering &_rendering,
                                                const Engine::Parameters::Aspects::TimeAspect::Time &_time,
                                                const float &_dest)
{
  switch(_rendering.m_renderClock)
  {
    case C15::Descriptors::SmootherClock::Sync:
      m_poly[_layer].start_sync(_rendering.m_renderIndex, _dest);
      break;
    case C15::Descriptors::SmootherClock::Audio:
      m_poly[_layer].start_audio(_rendering.m_renderIndex, _time.m_dxAudio, _dest);
      break;
    case C15::Descriptors::SmootherClock::Fast:
      m_poly[_layer].start_fast(_rendering.m_renderIndex, _time.m_dxFast, _dest);
      break;
    case C15::Descriptors::SmootherClock::Slow:
      m_poly[_layer].start_slow(_rendering.m_renderIndex, _time.m_dxSlow, _dest);
      break;
  }
  if constexpr(LOG_TRANSITIONS)
    nltools::Log::info(__PRETTY_FUNCTION__, "(layer:", _layer, "index:", _rendering.m_renderIndex,
                       ", clock:", (int) _rendering.m_renderClock, ", scaled:", _dest, ")");
}

inline void dsp_host_dual::monophonicTransition(const uint32_t &_layer,
                                                const Engine::Parameters::Aspects::RenderAspect::Rendering &_rendering,
                                                const Engine::Parameters::Aspects::TimeAspect::Time &_time,
                                                const float &_dest)
{
  switch(_rendering.m_renderClock)
  {
    case C15::Descriptors::SmootherClock::Sync:
      m_mono[_layer].start_sync(_rendering.m_renderIndex, _dest);
      break;
    case C15::Descriptors::SmootherClock::Audio:
      m_mono[_layer].start_audio(_rendering.m_renderIndex, _time.m_dxAudio, _dest);
      break;
    case C15::Descriptors::SmootherClock::Fast:
      m_mono[_layer].start_fast(_rendering.m_renderIndex, _time.m_dxFast, _dest);
      break;
    case C15::Descriptors::SmootherClock::Slow:
      m_mono[_layer].start_slow(_rendering.m_renderIndex, _time.m_dxSlow, _dest);
      break;
  }
  if constexpr(LOG_TRANSITIONS)
    nltools::Log::info(__PRETTY_FUNCTION__, "(layer:", _layer, "index:", _rendering.m_renderIndex,
                       ", clock:", (int) _rendering.m_renderClock, ", scaled:", _dest, ")");
}

bool dsp_host_dual::determinePolyChg(const C15::Properties::LayerId _layerId,
                                     const nltools::controls::PolyphonicUnmodulateableParameter &_unisonVoices,
                                     const nltools::controls::PolyphonicUnmodulateableParameter &_monoEnable)
{
  const uint32_t layerId = (uint32_t) _layerId;
  auto &unisonVoices = m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[IndexOfUnisonVoices];
  const bool unisonChanged = unisonVoices.update_position((float) _unisonVoices.m_controlPosition);
  auto &monoEnable = m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[IndexOfMonoEnable];
  const bool monoChanged = monoEnable.update_position((float) _monoEnable.m_controlPosition);
  return unisonChanged || monoChanged;
}

void dsp_host_dual::evalVoiceFadeChg(const uint32_t _layer)
{

  /// final version (when fade from range fits 128 keys):
  //  const float from = m_parameters.m_layer[_layer]
  //                         .m_unmodulateables[IndexOfVoiceGrpFadeFrom]
  //                         .m_scaled
  //      + C15::Config::physical_key_from;

  /// temporary version (for 61 key fade from range)
  // this little hack will treat edge fade from cases (Part I: C1, Part II: C6)
  // as virtual edge cases (keys 0, 127), ensuring that keys out of the 61 key
  // range can be heard (instead of being faded out)
  const uint32_t edgeCaseHackKey[2] = { C15::Config::physical_key_to, C15::Config::physical_key_from },
                 edgeCaseHackRemap[2] = { C15::Config::virtual_key_to, C15::Config::virtual_key_from };

  uint32_t from = static_cast<uint32_t>(
                      m_parameters.m_layer[_layer].m_polyphonic.m_unmodulateables[IndexOfVoiceGrpFadeFrom].m_scaled)
      + C15::Config::physical_key_from;

  const float range = m_parameters.m_layer[_layer].m_polyphonic.m_unmodulateables[IndexOfVoiceGrpFadeRange].m_scaled;

  if((from == edgeCaseHackKey[_layer]) && (range == 0.0f))
  {
    from = edgeCaseHackRemap[_layer];
  }
  ///
  if constexpr(VOICE_FADE_INTERPOLATION)
  {
    //      m_poly[_layer].evalVoiceFadeInterpolated(from, range);
    m_poly[_layer].evalVoiceFadeInterpolated(static_cast<float>(from), range);
  }
  else
  {
    //      m_poly[_layer].evalVoiceFade(from, range);
    m_poly[_layer].evalVoiceFade(static_cast<float>(from), range);
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::determineOutputEventSource(const bool _detected,
                                                                               const LayerMode _type)
{
  if(!_detected)
    return OutputResetEventSource::None;
  if(_type != LayerMode::Split)
    return OutputResetEventSource::Global;
  switch(m_alloc.m_internal_keys.pressedLocalKeys())
  {
    case AllocatorId::Local_I:
      return OutputResetEventSource::Local_I;
    case AllocatorId::Local_II:
      return OutputResetEventSource::Local_II;
    case AllocatorId::Local_Both:
      return OutputResetEventSource::Local_Both;
    default:
      break;
  }
  return OutputResetEventSource::None;
}

template <typename T> inline void dsp_host_dual::recallCommon(const T &_msg, const bool _resetVoiceFade)
{
  // reset
  m_parameters.m_global.m_parameters.m_assignment.reset();
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    // macro assignments
    m_parameters.m_layer[layerId].m_polyphonic.m_assignment.reset();
    m_parameters.m_layer[layerId].m_monophonic.m_assignment.reset();
    // voice fade
    if(_resetVoiceFade)
      m_poly[layerId].resetVoiceFade();
  }
  // global updates: hw sources
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: hw sources:");
  for(const auto &param : _msg.m_hardwareSources)
    onParameterRecall(param);
  // global updates: hw amounts
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: hw amounts:");
  for(const auto &param : _msg.m_hardwareAmounts)
    onParameterRecall(param);
  // global updates: macros
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: macro controls/times:");
  for(const auto &param : _msg.m_macroControls)
    onParameterRecall(param);
  for(const auto &param : _msg.m_macroTimes)
    onParameterRecall(param);
  // global updates: parameters
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: global params (modulateables/unmodulateables):");
  for(const auto &param : _msg.m_globalModulateables)
    onParameterRecall(param);
  for(const auto &param : _msg.m_globalUnmodulateables)
    onParameterRecall(param);
  // mono updates: parameters
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: mono params (modulateables/unmodulateables):");
  for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
  {
    for(const auto &param : _msg.m_monophonicModulateables[layer])
      onParameterRecall(layer, param);
    for(const auto &param : _msg.m_monophonicUnmodulateables[layer])
      onParameterRecall(layer, param);
  }
}

inline void dsp_host_dual::recallCommonTransition()
{
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: start transitions:");
  // start transitions: global modulateables/unmodulateables
  for(uint32_t i = 0; i < C15::Parameters::num_of_Global_Modulateables; i++)
  {
    const auto &param = m_parameters.m_global.m_parameters.m_modulateables[i];
    switch(i)
    {
      case IndexOfEffectsMix:
        if(m_layer_mode == LayerMode::Single)
          fxMixTransition(m_transitionTime.m_time, param.m_scaled);
        break;
      default:
        globalTransition(param.m_rendering, m_transitionTime.m_time, param.m_scaled);
        break;
    }
  }
  for(uint32_t i = 0; i < C15::Parameters::num_of_Global_Unmodulateables; i++)
  {
    const auto &param = m_parameters.m_global.m_parameters.m_unmodulateables[i];
    switch(i)
    {
      case IndexOfBaseKey:
        m_global.start_base_key(m_transitionTime.m_time.m_dxSlow, param.m_scaled);
        break;
      default:
        globalTransition(param.m_rendering, m_transitionTime.m_time, param.m_scaled);
        break;
    }
  }
  // start transitions: monophonic modulateables/unmodulateables
  for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
  {
    for(const auto &param : m_parameters.m_layer[layer].m_monophonic.m_modulateables)
      localTransition(layer, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
    for(const auto &param : m_parameters.m_layer[layer].m_monophonic.m_unmodulateables)
      localTransition(layer, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
  }
}

DSPInterface::OutputResetEventSource dsp_host_dual::recallSingle(const nltools::msg::SinglePresetMessage &_msg)
{
  const auto oldLayerMode = std::exchange(m_layer_mode, LayerMode::Single);
  const auto layerChanged = oldLayerMode != m_layer_mode;
  if constexpr(LOG_RECALL)
    nltools::Log::info(__PRETTY_FUNCTION__, "(@", m_clock.m_index, ")");
  // determine poly change (updating unison voices, mono enable)
  const auto polyChanged
      = determinePolyChg(C15::Properties::LayerId::I, _msg.m_polyphonicUnmodulateables[IndexOfUnisonVoices],
                         _msg.m_polyphonicUnmodulateables[IndexOfMonoEnable]);
  // reset detection
  const bool internalReset = layerChanged || polyChanged;
  const bool externalReset = internalReset && areKeysPressed(fromType(oldLayerMode));
  const OutputResetEventSource outputEvent = determineOutputEventSource(externalReset, oldLayerMode);
  if(internalReset)
  {
    if constexpr(LOG_RESET)
      nltools::Log::info("recall single voice reset");
    m_alloc.m_internal_keys.init();  // reset all pressed keys
    m_alloc.setUnison(0, m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[IndexOfUnisonVoices].m_position,
                      oldLayerMode, m_layer_mode);
    m_alloc.setMonoEnable(0, m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[IndexOfMonoEnable].m_position,
                          m_layer_mode);
    const uint32_t uVoice = m_alloc.m_unison - 1;
    for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
    {
      m_poly[layerId].resetEnvelopes();
      m_poly[layerId].m_uVoice = uVoice;
      m_poly[layerId].m_key_active = 0;
    }
  }
  // common updates: parameters, resetting voice fade buffer
  recallCommon(_msg, true);
  // polyphonic updates: parameters
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: polyphonic modulateables/unmodulateables:");
  for(const auto &param : _msg.m_polyphonicModulateables)
    onParameterRecall(0, param);
  for(const auto &param : _msg.m_polyphonicUnmodulateables)
    onParameterRecall(0, param, true);
  // start transitions: common
  recallCommonTransition();
  // start transitions: polyphonic modulateables/unmodulateables
  for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Modulateables; i++)
  {
    const auto &param = m_parameters.m_layer[0].m_polyphonic.m_modulateables[i];
    switch(i)
    {
      case IndexOfVoiceGrpVolume:  // ignore (single)
        break;
      case IndexOfMixEffectsFrom:
      case IndexOfMixToEffects:
        localTransition(0, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
        localTransition(1, param.m_rendering, m_transitionTime.m_time, 1.0f - param.m_scaled);
        break;
      default:
        for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
          localTransition(layer, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
        break;
    }
  }
  for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Unmodulateables; i++)
  {
    const auto &param = m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[i];
    for(uint32_t layer = 0; layer < C15::Properties::num_of_VoiceGroups; layer++)
      localTransition(layer, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
  }
  // logging levels after recall for debugging switching dual modes
  if constexpr(LOG_RECALL_LEVELS)
    debugLevels();
  // return detected reset event
  return outputEvent;
}

DSPInterface::OutputResetEventSource dsp_host_dual::recallSplit(const nltools::msg::SplitPresetMessage &_msg)
{
  const auto oldLayerMode = std::exchange(m_layer_mode, LayerMode::Split);
  const auto layerChanged = oldLayerMode != m_layer_mode;
  if constexpr(LOG_RECALL)
    nltools::Log::info(__PRETTY_FUNCTION__, "(@", m_clock.m_index, ")");
  // #3009: prepare reset detection with pressed local keys
  bool internalReset[2] = { m_alloc.m_internal_keys.pressedLocalKeys(0), m_alloc.m_internal_keys.pressedLocalKeys(1) };
  const bool externalReset = layerChanged && areKeysPressed(fromType(oldLayerMode));
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    const auto layer = static_cast<C15::Properties::LayerId>(layerId);

    // determine poly change (updating unison voices, mono enable)
    const auto polyChanged = determinePolyChg(layer, _msg.m_polyphonicUnmodulateables[layerId][IndexOfUnisonVoices],
                                              _msg.m_polyphonicUnmodulateables[layerId][IndexOfMonoEnable]);
    internalReset[layerId] &= polyChanged;
    // reset detection
    if((layerChanged || polyChanged))
    {
      if constexpr(LOG_RESET)
        nltools::Log::info("recall split voice reset(layerId:", layerId, ")");

      m_alloc.m_internal_keys.m_local[layerId] = 0;  // reset all pressed keys in part
      m_alloc.setUnison(0, m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[IndexOfUnisonVoices].m_position,
                        oldLayerMode, m_layer_mode);
      m_alloc.setMonoEnable(
          0, m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[IndexOfMonoEnable].m_position, m_layer_mode);
      const uint32_t uVoice = m_alloc.m_unison - 1;
      m_poly[layerId].resetEnvelopes();
      m_poly[layerId].m_uVoice = uVoice;
      m_poly[layerId].m_key_active = 0;
    }
  }
  // common updates: parameters, resetting voice fade buffer
  recallCommon(_msg, true);
  // polyphonic updates: parameters
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: polyphonic modulateables/unmodulateables:");
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    // polyphonic updates: modulateables/unmodulateables
    for(const auto &param : _msg.m_polyphonicModulateables[layerId])
      onParameterRecall(layerId, param);
    for(const auto &param : _msg.m_polyphonicUnmodulateables[layerId])
      onParameterRecall(layerId, param, true);
  }
  // start transitions: common
  recallCommonTransition();
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    // start transitions: polyphonic modulateables/unmodulateables
    for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Modulateables; i++)
    {
      const auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_modulateables[i];
      localTransition(layerId, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
    }
    for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Unmodulateables; i++)
    {
      const auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[i];
      localTransition(layerId, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
    }
  }
  // logging levels after recall for debugging switching dual modes
  if constexpr(LOG_RECALL_LEVELS)
    debugLevels();
  if(layerChanged)
  {
    // non-split -> split: (global or none)
    m_alloc.m_internal_keys.m_global = 0;  // reset all pressed global keys
    return determineOutputEventSource(externalReset, oldLayerMode);
  }
  // split -> split: determine actual outputEvent (I, II: pressed keys && poly
  // change)
  return m_alloc.m_internal_keys.pressedLocalKeys(internalReset[0], internalReset[1]);
}

DSPInterface::OutputResetEventSource dsp_host_dual::recallLayer(const nltools::msg::LayerPresetMessage &_msg)
{
  const auto oldLayerMode = std::exchange(m_layer_mode, LayerMode::Layer);
  const auto layerChanged = oldLayerMode != m_layer_mode;
  if constexpr(LOG_RECALL)
    nltools::Log::info(__PRETTY_FUNCTION__, "(@", m_clock.m_index, ")");
  // determine poly change (updating unison voices, mono enable)
  const auto polyChanged
      = determinePolyChg(C15::Properties::LayerId::I, _msg.m_polyphonicUnmodulateables[0][IndexOfUnisonVoices],
                         _msg.m_polyphonicUnmodulateables[0][IndexOfMonoEnable]);
  // reset detection
  const bool internalReset = layerChanged || polyChanged;
  const bool externalReset = internalReset && areKeysPressed(fromType(oldLayerMode));
  const OutputResetEventSource outputEvent = determineOutputEventSource(externalReset, oldLayerMode);
  if(internalReset)
  {
    if constexpr(LOG_RESET)
      nltools::Log::info("recall layer voice reset");
    m_alloc.m_internal_keys.init();  // reset all pressed keys
    m_alloc.setUnison(0, m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[IndexOfUnisonVoices].m_position,
                      oldLayerMode, m_layer_mode);
    m_alloc.setMonoEnable(0, m_parameters.m_layer[0].m_polyphonic.m_unmodulateables[IndexOfMonoEnable].m_position,
                          m_layer_mode);
    const uint32_t uVoice = m_alloc.m_unison - 1;
    for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
    {
      m_poly[layerId].resetEnvelopes();
      m_poly[layerId].m_uVoice = uVoice;
      m_poly[layerId].m_key_active = 0;
    }
  }
  // common updates: parameters, not resetting voice fade buffer
  recallCommon(_msg, false);
  // polyphonic updates: parameters
  if constexpr(LOG_RECALL)
    nltools::Log::info("recall: polyphonic modulateables/unmodulateables:");
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    const bool updateVoiceAlloc = layerId == 0;
    // polyphonic updates: modulateables/unmodulateables
    for(const auto &param : _msg.m_polyphonicModulateables[layerId])
      onParameterRecall(layerId, param);
    for(const auto &param : _msg.m_polyphonicUnmodulateables[layerId])
      onParameterRecall(layerId, param, updateVoiceAlloc);
    // re-calculate voice fade buffer
    evalVoiceFadeChg(layerId);
  }
  // start transitions: common
  recallCommonTransition();
  for(uint32_t layerId = 0; layerId < C15::Properties::num_of_VoiceGroups; layerId++)
  {
    // start transitions: polyphonic modulateables/unmodulateables
    for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Modulateables; i++)
    {
      const auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_modulateables[i];
      localTransition(layerId, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
    }
    for(uint32_t i = 0; i < C15::Parameters::num_of_Polyphonic_Unmodulateables; i++)
    {
      const auto &param = m_parameters.m_layer[layerId].m_polyphonic.m_unmodulateables[i];
      localTransition(layerId, param.m_rendering, m_transitionTime.m_time, param.m_scaled);
    }
  }
  // logging levels after recall for debugging switching dual modes
  if constexpr(LOG_RECALL_LEVELS)
    debugLevels();
  // return detected reset event
  return outputEvent;
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::HardwareSourceParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_global.m_hardwareSources[descriptor.m_param.m_index];
  param.update_behavior(getBehavior(_param.m_returnMode));
  param.update_position((float) _param.m_controlPosition);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::HardwareAmountParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_global.m_hardwareAmounts[descriptor.m_param.m_index];
  param.update_position((float) _param.m_controlPosition);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::MacroControlParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_global.m_macroControls[descriptor.m_param.m_index];
  param.update_position((float) _param.m_controlPosition);
  param.m_unclipped = param.m_position;  // fixing #2023: unclipped always up-to-date
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::MacroTimeParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_global.m_macroTimes[descriptor.m_param.m_index];
  param.update_position((float) _param.m_controlPosition);
  param.m_scaled = scale(param.m_scaling, param.m_position);
  updateTime(param.m_time, param.m_scaled);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::GlobalModulateableParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  const auto macroId = getMacroId(_param.m_macro);
  auto &param = m_parameters.m_global.m_parameters.m_modulateables[descriptor.m_param.m_index];
  param.update_source(getMacro(_param.m_macro));
  param.update_amount((float) _param.m_modulationAmount);
  param.update_position(param.depolarize((float) _param.m_controlPosition));
  param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
  m_parameters.m_global.m_parameters.m_assignment.reassign(descriptor.m_param.m_index, macroId);
  param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const nltools::controls::GlobalUnmodulateableParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_global.m_parameters.m_unmodulateables[descriptor.m_param.m_index];
  param.update_position((float) _param.m_controlPosition);
  param.m_scaled = scale(param.m_scaling, param.m_position);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const uint32_t &_layerId,
                                             const nltools::controls::PolyphonicModulateableParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  const auto macroId = getMacroId(_param.m_macro);
  auto &param = m_parameters.m_layer[_layerId].m_polyphonic.m_modulateables[descriptor.m_param.m_index];
  param.update_source(getMacro(_param.m_macro));
  param.update_amount((float) _param.m_modulationAmount);
  param.update_position(param.depolarize((float) _param.m_controlPosition));
  param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
  m_parameters.m_layer[_layerId].m_polyphonic.m_assignment.reassign(descriptor.m_param.m_index, macroId);
  param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
  if(param.m_splitpoint)
    m_alloc.setSplitPoint(static_cast<uint32_t>(param.m_scaled) + C15::Config::physical_key_from, _layerId);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const uint32_t &_layerId,
                                             const nltools::controls::PolyphonicUnmodulateableParameter &_param,
                                             const bool _vaUpdate)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_layer[_layerId].m_polyphonic.m_unmodulateables[descriptor.m_param.m_index];
  switch(_param.m_id)
  {
    case C15::PID::Unison_Voices:
    case C15::PID::Mono_Grp_Enable:
      break;
    default:
      param.update_position((float) _param.m_controlPosition);
      break;
  }
  param.m_scaled = scale(param.m_scaling, param.m_position);
  if(_vaUpdate)
    switch(_param.m_id)
    {
      case C15::PID::Mono_Grp_Prio:
        m_alloc.setMonoPriority(_layerId, param.m_scaled, m_layer_mode);
        break;
      case C15::PID::Mono_Grp_Legato:
        m_alloc.setMonoLegato(_layerId, param.m_scaled, m_layer_mode);
        break;
      default:
        break;
    }
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const uint32_t &_layerId,
                                             const nltools::controls::MonophonicModulateableParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  const auto macroId = getMacroId(_param.m_macro);
  auto &param = m_parameters.m_layer[_layerId].m_monophonic.m_modulateables[descriptor.m_param.m_index];
  param.update_source(getMacro(_param.m_macro));
  param.update_amount((float) _param.m_modulationAmount);
  param.update_position(param.depolarize((float) _param.m_controlPosition));
  param.m_scaled = scale(param.m_scaling, param.polarize(param.m_position));
  m_parameters.m_layer[_layerId].m_monophonic.m_assignment.reassign(descriptor.m_param.m_index, macroId);
  param.update_modulation_aspects(m_parameters.m_global.m_macroControls[macroId].m_position);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

inline void dsp_host_dual::onParameterRecall(const uint32_t &_layerId,
                                             const nltools::controls::MonophonicUnmodulateableParameter &_param)
{
  const auto &descriptor = getParameter(_param.m_id);
  auto &param = m_parameters.m_layer[_layerId].m_monophonic.m_unmodulateables[descriptor.m_param.m_index];
  param.update_position((float) _param.m_controlPosition);
  param.m_scaled = scale(param.m_scaling, param.m_position);
  if constexpr(LOG_RECALL_DETAILS)
    param.log(__PRETTY_FUNCTION__, descriptor);
}

void dsp_host_dual::debugLevels()
{
  const float masterVolume = m_parameters.m_global.m_parameters
                                 .m_modulateables[(uint32_t) C15::Parameters::Global_Modulateables::Master_Volume]
                                 .m_scaled;
  const float voiceGroupVolume[2]
      = { m_parameters.m_layer[0].m_polyphonic.m_modulateables[IndexOfVoiceGrpVolume].m_scaled,
          m_parameters.m_layer[1].m_polyphonic.m_modulateables[IndexOfVoiceGrpVolume].m_scaled };
  nltools::Log::info(__PRETTY_FUNCTION__, "Master:", masterVolume, "VoiceGroup[I]:", voiceGroupVolume[0],
                     "VoiceGroup[II]:", voiceGroupVolume[1]);
}

void dsp_host_dual::onHWChanged(HardwareSource id, float value, bool didBehaviourChange)
{
  if(id != HardwareSource::NONE)
  {
    auto idx = static_cast<int>(id);
    auto &source = m_parameters.m_global.m_hardwareSources[idx];
    const float inc = value - source.m_position;
    source.m_position = value;
    if(!didBehaviourChange)
      hwModChain(source, idx, inc);
  }
}

C15::Properties::HW_Return_Behavior dsp_host_dual::getBehaviour(HardwareSource id)
{
  if(id != HardwareSource::NONE)
  {
    auto idx = static_cast<int>(id);
    return m_parameters.m_global.m_hardwareSources[idx].m_behavior;
  }
  return C15::Properties::HW_Return_Behavior::Stay;
}

SoundType dsp_host_dual::getType()
{
  // a little inconvenient and redundant...
  switch(m_layer_mode)
  {
    case LayerMode::Single:
      return SoundType::Single;
    case LayerMode::Split:
      return SoundType::Split;
    case LayerMode::Layer:
      return SoundType::Layer;
  }
  nltools_detailedAssertAlways(true, __PRETTY_FUNCTION__);
  return SoundType::Invalid;  // should never be reached
}

VoiceGroup dsp_host_dual::getSplitPartForKeyDown(int key)
{
  const auto allocID = m_alloc.getSplitPartForKeyDown(key);
  return getVoiceGroupFromAllocatorId(allocID);
}

VoiceGroup dsp_host_dual::getSplitPartForKeyUp(int key, InputEventSource from)
{
  const auto allocID = m_alloc.getSplitPartForKeyUp(key, getInputSourceId(from));
  return getVoiceGroupFromAllocatorId(allocID);
}

VoiceGroup dsp_host_dual::getNonLocalSplitKeyAssignmentForKeyUp(int key)
{
  const auto allocID = m_alloc.getNonlocalSplitPartForKeyUp(key);
  return getVoiceGroupFromAllocatorId(allocID);
}

void dsp_host_dual::registerNonLocalKeyAssignment(const int note, VoiceGroup part)
{
  // register key assignment despite local off (similar to keyDown/splitKeyDown,
  // but not quite)
  if(m_layer_mode == LayerMode::Split)
  {
    switch(part)
    {
      case VoiceGroup::I:  // applies to Part I only
        m_alloc.registerNonLocalKeyAssignment(note, AllocatorId::Local_I);
        break;
      case VoiceGroup::II:  // applies to Part II only
        m_alloc.registerNonLocalKeyAssignment(note, AllocatorId::Local_II);
        break;
      case VoiceGroup::Global:  // applies to both Parts I, II at once
        m_alloc.registerNonLocalKeyAssignment(note, AllocatorId::Local_Both);
        break;
      default:
      case VoiceGroup::NumGroups:
      case VoiceGroup::Invalid:
        break;
    }
  }
  else
  {
    m_alloc.registerNonLocalKeyAssignment(note, AllocatorId::Global);
  }
}

void dsp_host_dual::unregisterNonLocalKeyAssignment(const int note)
{
  // unregister key assignment despite local off (similar to keyUp/splitKeyUp,
  // but not quite)
  m_alloc.unregisterNonLocalKeyAssignment(note);
}

void dsp_host_dual::onKeyDown(const int note, float velocity, InputEventSource from)
{
  // InputEvent can be singular (TCD or Primary) or separate (Primary or
  // Secondary or Both) Secondary only exists in Split Sounds, so the final
  // sourceId should be either TCD (0) or Primary (1)
  const uint32_t inputSourceId = from == InputEventSource::Internal ? 0 : 1;

  bool valid = false;
  switch(m_layer_mode)
  {
    case LayerMode::Single:
      valid = m_alloc.onSingleKeyDown(note, velocity, inputSourceId);
      break;
    case LayerMode::Layer:
      valid = m_alloc.onLayerKeyDown(note, velocity, inputSourceId);
      break;
    default:
      break;
  }
  if(valid)
    keyDownTraversal(note, velocity, inputSourceId);
  else if constexpr(LOG_FAIL)
    nltools::Log::warning(__PRETTY_FUNCTION__, "keyDown(src:", inputSourceId, ", pos:", note, ") failed!");
}

void dsp_host_dual::onKeyUp(const int note, float velocity, InputEventSource from)
{
  // InputEvent can be singular (TCD or Primary) or separate (Primary or
  // Secondary or Both) Secondary only exists in Split Sounds, so the final
  // sourceId should be either TCD (0) or Primary (1)
  const uint32_t inputSourceId = from == InputEventSource::Internal ? 0 : 1;

  bool valid = false;
  switch(m_layer_mode)
  {
    case LayerMode::Single:
      valid = m_alloc.onSingleKeyUp(note, velocity, inputSourceId);
      break;
    case LayerMode::Layer:
      valid = m_alloc.onLayerKeyUp(note, velocity, inputSourceId);
      break;
    default:
      break;
  }
  if(valid)
    keyUpTraversal(note, velocity, inputSourceId);
  else if constexpr(LOG_FAIL)
    nltools::Log::warning(__PRETTY_FUNCTION__, "keyUp(src:", inputSourceId, ", pos:", note, ") failed!");
}

void dsp_host_dual::onKeyDownSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEventSource from)
{
  const uint32_t inputSourceId = getInputSourceId(from);
  bool valid = false;
  if(m_layer_mode == LayerMode::Split)
  {
    switch(part)
    {
      case VoiceGroup::I:  // applies to Part I only
        valid = m_alloc.onSplitKeyDown(note, velocity, inputSourceId, AllocatorId::Local_I);
        break;
      case VoiceGroup::II:  // applies to Part II only
        valid = m_alloc.onSplitKeyDown(note, velocity, inputSourceId, AllocatorId::Local_II);
        break;
      case VoiceGroup::Global:  // applies to both Parts I, II at once
        valid = m_alloc.onSplitKeyDown(note, velocity, inputSourceId, AllocatorId::Local_Both);
        break;
      default:
      case VoiceGroup::NumGroups:
      case VoiceGroup::Invalid:
        valid = false;
        break;
    }
  }
  if(valid)
    keyDownTraversal(note, velocity, inputSourceId);
  else if constexpr(LOG_FAIL)
    nltools::Log::warning(__PRETTY_FUNCTION__, "keyDown(src:", inputSourceId, ", pos:", note, ") failed!");
}

void dsp_host_dual::onKeyUpSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEventSource from)
{
  const uint32_t inputSourceId = getInputSourceId(from);
  bool valid = false;
  if(m_layer_mode == LayerMode::Split)
  {
    switch(part)
    {
      case VoiceGroup::I:  // applies to Part I only
        valid = m_alloc.onSplitKeyUp(note, velocity, inputSourceId, AllocatorId::Local_I);
        break;
      case VoiceGroup::II:  // applies to Part II only
        valid = m_alloc.onSplitKeyUp(note, velocity, inputSourceId, AllocatorId::Local_II);
        break;
      case VoiceGroup::Global:  // applies to both Parts I, II at once
        valid = m_alloc.onSplitKeyUp(note, velocity, inputSourceId, AllocatorId::Local_Both);
        break;
      default:
      case VoiceGroup::NumGroups:
      case VoiceGroup::Invalid:
        valid = false;
        break;
    }
  }
  if(valid)
    keyUpTraversal(note, velocity, inputSourceId);
  else if constexpr(LOG_FAIL)
    nltools::Log::warning(__PRETTY_FUNCTION__, "keyUp(src:", inputSourceId, ", pos:", note, ") failed!");
}

void dsp_host_dual::fadeOutResetVoiceAllocAndEnvelopes()
{
  m_fade.muteAndDo(
      [&]
      {
        m_alloc.reset();
        for(auto &layerId : m_poly)
        {
          layerId.resetEnvelopes();
          layerId.m_key_active = 0;
        }
      });
}

bool dsp_host_dual::updateBehaviour(C15::ParameterDescriptor &element, ReturnMode mode)
{
  auto &param = m_parameters.m_global.m_hardwareSources[element.m_param.m_index];
  return param.update_behavior(getBehavior(mode));
}

float dsp_host_dual::getReturnValueFor(HardwareSource hwid)
{
  return 0;
}

void dsp_host_dual::resetReturningHWSource(HardwareSource hwui)
{
  // TODO implement reset!!
  DSPInterface::resetReturningHWSource(hwui);
}

bool dsp_host_dual::areKeysPressed(SoundType _current)
{
  switch(_current)
  {
    case SoundType::Layer:
    case SoundType::Single:
      return m_alloc.m_internal_keys.m_global > 0;
    case SoundType::Split:
      return (m_alloc.m_internal_keys.m_local[0] + m_alloc.m_internal_keys.m_local[1]) > 0;
    default:
      break;
  }
  return false;
}
