#include "C15Synth.h"
#include "c15-audio-engine/dsp_host_dual.h"
#include "main.h"
#include "Options.h"

#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>

C15Synth::C15Synth()
    : m_dsp(std::make_unique<dsp_host_dual>())
{
  m_dsp->init(getOptions()->getSampleRate(), getOptions()->getPolyphony());

  using namespace nltools::msg;

  receive<SinglePresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onSinglePresetMessage));
  receive<LayerPresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onLayerPresetMessage));
  receive<SplitPresetMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onSplitPresetMessage));

  receive<UnmodulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                                 sigc::mem_fun(this, &C15Synth::onUnmodulateableParameterMessage));
  receive<ModulateableParameterChangedMessage>(EndPoint::AudioEngine,
                                               sigc::mem_fun(this, &C15Synth::onModulateableParameterMessage));
  receive<MacroControlChangedMessage>(EndPoint::AudioEngine,
                                      sigc::mem_fun(this, &C15Synth::onMacroControlParameterMessage));
  receive<HWSourceChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWSourceMessage));
  receive<HWAmountChangedMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onHWAmountMessage));

  //Settings
  receive<Setting::NoteShiftMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onNoteShiftMessage));
  receive<Setting::PresetGlitchMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onPresetGlitchMessage));
  receive<Setting::TransitionTimeMessage>(EndPoint::AudioEngine,
                                          sigc::mem_fun(this, &C15Synth::onTransitionTimeMessage));
  receive<Setting::EditSmoothingTimeMessage>(EndPoint::AudioEngine,
                                             sigc::mem_fun(this, &C15Synth::onEditSmoothingTimeMessage));
}

C15Synth::~C15Synth() = default;

void C15Synth::doMidi(const MidiEvent &event)
{
#if test_inputModeFlag
  m_dsp->onRawMidiMessage(event.raw[0], event.raw[1], event.raw[2]);
#else
  m_dsp->onMidiMessage(event.raw[0], event.raw[1], event.raw[2]);
#endif
}

void C15Synth::doAudio(SampleFrame *target, size_t numFrames)
{
  for(size_t i = 0; i < numFrames; i++)
  {
    m_dsp->render();
    target[i].left = m_dsp->m_mainOut_L;
    target[i].right = m_dsp->m_mainOut_R;
  }
}

void C15Synth::printAndResetTcdInputLog()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::resetDSP()
{
  m_dsp->reset();
  nltools::Log::info("DSP has been reset.");
}

void C15Synth::toggleTestTone()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::selectTestToneFrequency()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::selectTestToneAmplitude()
{
  nltools::Log::info("currently disabled");
}

void C15Synth::increase()
{
  changeSelectedValueBy(1);
}

void C15Synth::decrease()
{
  changeSelectedValueBy(-1);
}

double C15Synth::measurePerformance(std::chrono::seconds time)
{
  for(int i = 0; i < getOptions()->getPolyphony(); i++)
  {
    m_dsp->onMidiMessage(1, 0, 53);
    m_dsp->onMidiMessage(5, 62, 64);
    m_dsp->onMidiMessage(1, 0, 83);
    m_dsp->onMidiMessage(5, 62, 64);
  }
  return Synth::measurePerformance(time);
}

void C15Synth::changeSelectedValueBy(int i)
{
  nltools::Log::info("currently disabled");
}

void C15Synth::onModulateableParameterMessage(const nltools::msg::ModulateableParameterChangedMessage &msg)
{
  //nltools::Log::info("Received modulateable parameter message!");
  // dispatch and safety check
  auto element = C15::ParameterList[msg.parameterId];
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Modulateable_Parameter)
  {
    float pos = static_cast<float>(msg.controlPosition), amt = static_cast<float>(msg.mcAmount);
    C15::Parameters::Macro_Controls src = static_cast<C15::Parameters::Macro_Controls>(msg.sourceMacro);
    switch(msg.voiceGroup)
    {
      case VoiceGroup::I:
        m_dsp->update_event_target_param(element.m_param.m_index, 0, msg.lock, pos, src, amt);
        break;
      case VoiceGroup::II:
        m_dsp->update_event_target_param(element.m_param.m_index, 1, msg.lock, pos, src, amt);
        break;
    }
  }
  else
  {
    nltools::Log::info("invalid Modulateable_Parameter ID:", msg.parameterId);
  }
}

void C15Synth::onUnmodulateableParameterMessage(const nltools::msg::UnmodulateableParameterChangedMessage &msg)
{
  //nltools::Log::info("Received unmodulateable parameter message!");
  // dispatch and safety check
  auto element = C15::ParameterList[msg.parameterId];
  switch(element.m_param.m_type)
  {
    case C15::Descriptors::ParameterType::Global_Parameter:
      m_dsp->update_event_global_param(element.m_param.m_index, msg.lock, static_cast<float>(msg.controlPosition));
      break;
    case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
      switch(msg.voiceGroup)
      {
        case VoiceGroup::I:
          m_dsp->update_event_direct_param(element.m_param.m_index, 0, msg.lock,
                                           static_cast<float>(msg.controlPosition));
          break;
        case VoiceGroup::II:
          m_dsp->update_event_direct_param(element.m_param.m_index, 1, msg.lock,
                                           static_cast<float>(msg.controlPosition));
          break;
      }
      break;
    case C15::Descriptors::ParameterType::Macro_Time:
      switch(msg.voiceGroup)
      {
        case VoiceGroup::I:
          m_dsp->update_event_macro_time(element.m_param.m_index, 0, msg.lock, static_cast<float>(msg.controlPosition));
          break;
        case VoiceGroup::II:
          m_dsp->update_event_macro_time(element.m_param.m_index, 1, msg.lock, static_cast<float>(msg.controlPosition));
          break;
      }
      break;
    default:
      nltools::Log::info("invalid Unmodulateable_Parameter ID:", msg.parameterId);
      break;
  }
}

void C15Synth::onMacroControlParameterMessage(const nltools::msg::MacroControlChangedMessage &msg)
{
  //nltools::Log::info("Received macro control parameter message!");
  // dispatch and safety check
  auto element = C15::ParameterList[msg.parameterId];
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Macro_Control)
  {
    switch(msg.voiceGroup)
    {
      case VoiceGroup::I:
        m_dsp->update_event_macro_ctrl(element.m_param.m_index, 0, msg.lock, static_cast<float>(msg.controlPosition));
        break;
      case VoiceGroup::II:
        m_dsp->update_event_macro_ctrl(element.m_param.m_index, 1, msg.lock, static_cast<float>(msg.controlPosition));
        break;
    }
  }
  else
  {
    nltools::Log::info("invalid HW_Amount ID:", msg.parameterId);
  }
}

void C15Synth::onHWAmountMessage(const nltools::msg::HWAmountChangedMessage &msg)
{
  //nltools::Log::info("Received hwAmount parameter message!");
  // dispatch and safety check
  auto element = C15::ParameterList[msg.parameterId];
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Amount)
  {
    switch(msg.voiceGroup)
    {
      case VoiceGroup::I:
        m_dsp->update_event_hw_amount(element.m_param.m_index, 0, msg.lock, static_cast<float>(msg.controlPosition));
        break;
      case VoiceGroup::II:
        m_dsp->update_event_hw_amount(element.m_param.m_index, 1, msg.lock, static_cast<float>(msg.controlPosition));
        break;
    }
  }
  else
  {
    nltools::Log::info("invalid HW_Amount ID:", msg.parameterId);
  }
}

void C15Synth::onHWSourceMessage(const nltools::msg::HWSourceChangedMessage &msg)
{
  //nltools::Log::info("Received hwSource parameter message!");
  // dispatch and safety check
  auto element = C15::ParameterList[msg.parameterId];
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source)
  {
    const float pos = static_cast<float>(msg.controlPosition);
    // later: optimize and only use one single descriptive enum for that (probably from nltools)
    switch(msg.returnMode)
    {
      case ReturnMode::None:
        m_dsp->update_event_hw_source(element.m_param.m_index, msg.lock, C15::Properties::HW_Return_Behavior::Stay,
                                      pos);
        break;
      case ReturnMode::Center:
        m_dsp->update_event_hw_source(element.m_param.m_index, msg.lock, C15::Properties::HW_Return_Behavior::Center,
                                      pos);
        break;
      case ReturnMode::Zero:
        m_dsp->update_event_hw_source(element.m_param.m_index, msg.lock, C15::Properties::HW_Return_Behavior::Zero,
                                      pos);
        break;
    }
  }
  else
  {
    nltools::Log::info("invalid HW_Source ID:", msg.parameterId);
  }
}

void C15Synth::onSplitPresetMessage(const nltools::msg::SplitPresetMessage &msg)
{
  m_dsp->onPresetMessage(msg);
}

void C15Synth::onSinglePresetMessage(const nltools::msg::SinglePresetMessage &msg)
{
  m_dsp->onPresetMessage(msg);
}

void C15Synth::onLayerPresetMessage(const nltools::msg::LayerPresetMessage &msg)
{
  m_dsp->onPresetMessage(msg);
}

void C15Synth::onNoteShiftMessage(const nltools::msg::Setting::NoteShiftMessage &msg)
{
  m_dsp->update_event_note_shift(msg.m_shift);
}

void C15Synth::onPresetGlitchMessage(const nltools::msg::Setting::PresetGlitchMessage &msg)
{
  m_dsp->update_event_glitch_suppr(msg.m_enabled);
}

void C15Synth::onTransitionTimeMessage(const nltools::msg::Setting::TransitionTimeMessage &msg)
{
  m_dsp->update_event_transition_time(msg.m_value);
}

void C15Synth::onEditSmoothingTimeMessage(const nltools::msg::Setting::EditSmoothingTimeMessage &msg)
{
  m_dsp->update_event_edit_time(msg.m_time);
}
