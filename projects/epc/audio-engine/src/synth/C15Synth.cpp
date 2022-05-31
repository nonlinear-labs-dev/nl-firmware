#include "C15Synth.h"
#include "AudioEngineOptions.h"
#include "c15-audio-engine/dsp_host_dual.h"
#include <glibmm/main.h>
#include <nltools/logging/Log.h>
#include <nltools/messaging/Message.h>

constexpr static u_int8_t MIDI_PROGRAMCHANGE_PATTERN = 0b11000000;

C15Synth::C15Synth(AudioEngineOptions* options)
    : Synth(options)
    , m_dsp(std::make_unique<dsp_host_dual>())
    , m_options(options)
    , m_externalMidiOutBuffer(2048)
    , m_queuedChannelModeMessages(128)
    , m_syncExternalsTask(std::async(std::launch::async, [this] { syncExternalsLoop(); }))
    , m_inputEventStage { m_dsp.get(), &m_midiOptions, [this] { m_syncExternalsWaiter.notify_all(); },
                          [this](auto msg) { queueExternalMidiOut(msg); },
                          [this](MidiChannelModeMessages func) { queueChannelModeMessage(func); } }
{
  constexpr auto maxV = std::numeric_limits<float>::max();
  m_playgroundHwSourceKnownValues.fill({ maxV, maxV, maxV });

  m_dsp->init(options->getSampleRate(), options->getPolyphony());

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
  receive<HWSourceSendChangedMessage>(EndPoint::AudioEngine,
                                      sigc::mem_fun(this, &C15Synth::onHWSourceSendMessageReceived));
  //Settings
  receive<Setting::NoteShiftMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onNoteShiftMessage));
  receive<Setting::PresetGlitchMessage>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onPresetGlitchMessage));
  receive<Setting::TransitionTimeMessage>(EndPoint::AudioEngine,
                                          sigc::mem_fun(this, &C15Synth::onTransitionTimeMessage));
  receive<Setting::EditSmoothingTimeMessage>(EndPoint::AudioEngine,
                                             sigc::mem_fun(this, &C15Synth::onEditSmoothingTimeMessage));

  receive<Setting::TuneReference>(EndPoint::AudioEngine, sigc::mem_fun(this, &C15Synth::onTuneReferenceMessage));

  receive<Keyboard::NoteUp>(EndPoint::AudioEngine, [this](const Keyboard::NoteUp& noteUp) {
    m_inputEventStage.onMIDIMessage({ { 0, static_cast<uint8_t>(noteUp.m_keyPos), 0 } });
    m_syncExternalsWaiter.notify_all();
  });

  receive<Keyboard::NoteDown>(EndPoint::AudioEngine, [this](const Keyboard::NoteDown& noteDown) {
    m_inputEventStage.onMIDIMessage({ { 100, static_cast<uint8_t>(noteDown.m_keyPos), 0 } });
    m_syncExternalsWaiter.notify_all();
  });

  // receive program changes from playground and dispatch it to midi-over-ip
  receive<nltools::msg::Midi::ProgramChangeMessage>(EndPoint::AudioEngine, [this](const auto& pc) {
    bool scheduled = false;

    const int sendPrimChannel = MidiRuntimeOptions::channelEnumToInt(m_midiOptions.getMIDIPrimarySendChannel());
    if(sendPrimChannel != -1 && m_midiOptions.shouldSendMIDIProgramChangesOnPrimary())
    {
      const uint8_t newStatus = MIDI_PROGRAMCHANGE_PATTERN | sendPrimChannel;
      m_externalMidiOutBuffer.push(nltools::msg::Midi::SimpleMessage { newStatus, pc.program });
      scheduled = true;
    }

    const int sendSecChannel = MidiRuntimeOptions::channelEnumToInt(m_midiOptions.getMIDISplitSendChannel());
    if(sendSecChannel != -1 && m_midiOptions.shouldSendMIDIProgramChangesOnSplit())
    {
      const uint8_t newStatus = MIDI_PROGRAMCHANGE_PATTERN | sendSecChannel;
      m_externalMidiOutBuffer.push(nltools::msg::Midi::SimpleMessage { newStatus, pc.program });
      scheduled = true;
    }

    if(scheduled)
      m_syncExternalsWaiter.notify_all();
  });

  receive<nltools::msg::Midi::SimpleMessage>(EndPoint::ExternalMidiOverIPClient, [&](const auto& msg) {
    MidiEvent e;
    std::copy(msg.rawBytes.data(), msg.rawBytes.data() + msg.numBytesUsed, e.raw);

    const auto isPC = (e.raw[0] & 0xF0) == 0xC0;
    if(isPC)
    {
      const auto receivedChannel = static_cast<int>(e.raw[0]) - 192;
      const auto isPrimaryOmniReceive = m_midiOptions.getMIDIPrimaryReceiveChannel() == MidiReceiveChannel::Omni;
      const auto isSplitOmniReceive = m_midiOptions.getMIDISplitReceiveChannel() == MidiReceiveChannelSplit::Omni;

      const auto receivedChannelMatchesPrimary
          = MidiRuntimeOptions::channelEnumToInt(m_midiOptions.getMIDIPrimaryReceiveChannel()) == receivedChannel;
      const auto receivedChannelMatchedSplit
          = MidiRuntimeOptions::channelEnumToInt(m_midiOptions.getMIDISplitReceiveChannel()) == receivedChannel;

      if(isPrimaryOmniReceive || receivedChannelMatchesPrimary)
      {
        if(m_midiOptions.shouldReceiveMIDIProgramChangesOnPrimary())
        {
          send(nltools::msg::EndPoint::Playground, nltools::msg::Midi::ProgramChangeMessage { e.raw[1] });
        }
      }
      else if(isSplitOmniReceive || receivedChannelMatchedSplit)
      {
        if(m_midiOptions.shouldReceiveMIDIProgramChangesOnSplit())
        {
          send(nltools::msg::EndPoint::Playground, nltools::msg::Midi::ProgramChangeMessage { e.raw[1] });
        }
      }
    }
    else
    {
      pushMidiEvent(e);
    }
  });

  receive<nltools::msg::Setting::MidiSettingsMessage>(EndPoint::AudioEngine,
                                                      sigc::mem_fun(this, &C15Synth::onMidiSettingsMessage));

  receive<nltools::msg::PanicAudioEngine>(EndPoint::AudioEngine,
                                          sigc::mem_fun(this, &C15Synth::onPanicNotificationReceived));
}

C15Synth::~C15Synth()
{
  {
    std::unique_lock<std::mutex> inputEventStageLock(m_syncExternalsMutex);
    m_quit = true;
  }
  m_syncExternalsWaiter.notify_all();
  m_syncExternalsTask.wait();
}

dsp_host_dual* C15Synth::getDsp() const
{
  return m_dsp.get();
}

void C15Synth::syncExternalsLoop()
{
  std::unique_lock<std::mutex> lock(m_syncExternalsMutex);

  while(!m_quit)
  {
    m_syncExternalsWaiter.wait(lock);

    if(!m_quit)
    {
      doSyncExternalMidiBridge();
      doSyncPlayground();
      doChannelModeMessageFunctions();
    }
  }
}

void C15Synth::doChannelModeMessageFunctions()
{
  //TODO implement remaining special MIDI functions here
  while(!m_queuedChannelModeMessages.empty())
  {
    switch(m_queuedChannelModeMessages.pop())
    {
      case AllSoundOff:
        resetDSP();
        break;
      case ResetAllControllers:
        break;
      case LocalControllersOn:
      {
        nltools::msg::Setting::SetGlobalLocalSetting msg { true };
        nltools::msg::send(nltools::msg::EndPoint::Playground, msg);
      }
      break;
      case LocalControllersOff:
      {
        nltools::msg::Setting::SetGlobalLocalSetting msg { false };
        nltools::msg::send(nltools::msg::EndPoint::Playground, msg);
      }
      break;
      case AllNotesOff:
        m_dsp->fadeOutResetVoiceAllocAndEnvelopes();  // NOTE: currently resets all (internal AND external) notes
        break;
      case PollEnd:
      {
        nltools::msg::HardwareSourcePollEnd msg;
        msg.m_data = m_inputEventStage.getPolledHWSourcePositions();
        nltools::msg::send(nltools::msg::EndPoint::Playground, msg);
        break;
      }
      default:
      case NOOP:
        break;
    }
  }
}

void C15Synth::doSyncExternalMidiBridge()
{
  while(!m_externalMidiOutBuffer.empty())
  {
    auto msg = m_externalMidiOutBuffer.pop();
    auto copy = msg;
    send(nltools::msg::EndPoint::ExternalMidiOverIPBridge, copy);
  }
}

void C15Synth::doSyncPlayground()
{
  using namespace nltools::msg;

  if(m_inputEventStage.getAndResetKeyBedStatus())
  {
    send(EndPoint::Playground, Keyboard::NoteEventHappened {});
  }

  auto engineHWSourceValues = m_dsp->getHWSourceValues();

  for(auto hw : sHardwareSources)
  {
    const auto idx = static_cast<unsigned int>(hw);
    const auto isLocalEnabled = m_midiOptions.isLocalEnabled(hw);

    auto sendParameterValue = m_inputEventStage.getHWSourcePositionIfLocalDisabled(hw);
    auto audioParameterValue = engineHWSourceValues[idx];
    auto audioParameterSource = m_dsp->getHardwareSourceLastChangeSource(hw);
    const auto currentValue = isLocalEnabled ? audioParameterValue : sendParameterValue;
    const auto valueSource = isLocalEnabled ? audioParameterSource : m_inputEventStage.getHWSourcePositionSource(hw);

    if(std::exchange(m_playgroundHwSourceKnownValues[idx][static_cast<int>(valueSource)], currentValue) != currentValue)
    {
      HardwareSourceChangedNotification msg;
      msg.hwSource = idx;
      msg.source = valueSource;
      msg.position = static_cast<double>(currentValue);
      send(EndPoint::Playground, msg);
    }
  }
}

void C15Synth::doMidi(const MidiEvent& event)
{
  m_inputEventStage.onMIDIMessage(event);
}

void C15Synth::doTcd(const MidiEvent& event)
{
  m_inputEventStage.onTCDMessage(event);
}

unsigned int C15Synth::getRenderedSamples()
{
  return m_dsp->m_sample_counter;
}

void C15Synth::doAudio(SampleFrame* target, size_t numFrames)
{
#ifdef __arm__
#else
  // to avoid denormals, we exploit flush_to_zero within this code-block: every denormal float will be set to zero
  _mm_setcsr(_mm_getcsr() | (1 << 15) | (1 << 6));
  // then, the samples are rendered as before
#endif

  for(size_t i = 0; i < numFrames; i++)
  {
    m_dsp->render();
    target[i].left = m_dsp->m_mainOut_L;
    target[i].right = m_dsp->m_mainOut_R;
  }
}

void C15Synth::logStatus()
{
  m_dsp->logStatus();
}

void C15Synth::resetDSP()
{
  m_dsp->reset();
}

void C15Synth::toggleTestTone()
{
  m_dsp->onSettingToneToggle(0);  // 0 - switches modes
}

void C15Synth::onSettingInitialSinglePreset()
{
  m_dsp->onSettingInitialSinglePreset();  // initial sound, out a mix: 100%
}

void C15Synth::onModulateableParameterMessage(const nltools::msg::ModulateableParameterChangedMessage& msg)
{
  // (fail-safe) dispatch by ParameterList
  auto element = m_dsp->getParameter(msg.parameterId);
  switch(element.m_param.m_type)
  {
    case C15::Descriptors::ParameterType::Global_Modulateable:
      m_dsp->globalParChg(element.m_param.m_index, msg);
      return;
    case C15::Descriptors::ParameterType::Local_Modulateable:
      m_dsp->localParChg(element.m_param.m_index, msg);
      return;
  }
#if LOG_FAIL
  nltools::Log::warning("invalid Modulateable_Parameter ID:", msg.parameterId);
#endif
}

void C15Synth::onUnmodulateableParameterMessage(const nltools::msg::UnmodulateableParameterChangedMessage& msg)
{
  // (fail-safe) dispatch by ParameterList
  auto element = m_dsp->getParameter(msg.parameterId);
  // further (subtype) distinction
  switch(element.m_param.m_type)
  {
    case C15::Descriptors::ParameterType::Global_Unmodulateable:
      m_dsp->globalParChg(element.m_param.m_index, msg);
      return;
    case C15::Descriptors::ParameterType::Macro_Time:
      m_dsp->globalTimeChg(element.m_param.m_index, msg);
      return;
    case C15::Descriptors::ParameterType::Local_Unmodulateable:
      // poly detection
      switch(static_cast<C15::Parameters::Local_Unmodulateables>(element.m_param.m_index))
      {
        case C15::Parameters::Local_Unmodulateables::Unison_Voices:
        {
          const auto externalReset = m_dsp->localUnisonVoicesChg(msg);
          m_inputEventStage.requestExternalReset(externalReset);
          break;
        }
        case C15::Parameters::Local_Unmodulateables::Mono_Grp_Enable:
        {
          const auto externalReset = m_dsp->localMonoEnableChg(msg);
          m_inputEventStage.requestExternalReset(externalReset);
          break;
        }
        case C15::Parameters::Local_Unmodulateables::Mono_Grp_Prio:
          m_dsp->localMonoPriorityChg(msg);
          break;
        case C15::Parameters::Local_Unmodulateables::Mono_Grp_Legato:
          m_dsp->localMonoLegatoChg(msg);
          break;
        default:
          m_dsp->localParChg(element.m_param.m_index, msg);
          break;
      }
      return;
    default:
      break;
  }
#if LOG_FAIL
  nltools::Log::warning("invalid Unmodulateable_Parameter ID:", msg.parameterId);
#endif
}

void C15Synth::onMacroControlParameterMessage(const nltools::msg::MacroControlChangedMessage& msg)
{
  // (fail-safe) dispatch by ParameterList
  auto element = m_dsp->getParameter(msg.parameterId);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Macro_Control)
  {
    m_dsp->globalParChg(element.m_param.m_index, msg);
    return;
  }
#if LOG_FAIL
  nltools::Log::warning("invalid Macro_Control ID:", msg.parameterId);
#endif
}

void C15Synth::onHWAmountMessage(const nltools::msg::HWAmountChangedMessage& msg)
{
  // (fail-safe) dispatch by ParameterList
  auto element = m_dsp->getParameter(msg.parameterId);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Amount)
  {
    m_dsp->globalParChg(element.m_param.m_index, msg);
    return;
  }
#if LOG_FAIL
  nltools::Log::warning("invalid HW_Amount ID:", msg.parameterId);
#endif
}

void C15Synth::onHWSourceMessage(const nltools::msg::HWSourceChangedMessage& msg)
{
  auto element = m_dsp->getParameter(msg.parameterId);
  auto latchIndex = InputEventStage::parameterIDToHWID(msg.parameterId);

  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source && latchIndex != HardwareSource::NONE)
  {
    auto didBehaviourChange = m_dsp->updateBehaviour(element, msg.returnMode);
    m_playgroundHwSourceKnownValues[static_cast<int>(latchIndex)][static_cast<int>(HWChangeSource::UI)]
        = static_cast<float>(msg.controlPosition);
    m_inputEventStage.onUIHWSourceMessage(msg, didBehaviourChange);
  }
}

void C15Synth::queueChannelModeMessage(MidiChannelModeMessages function)
{
  m_queuedChannelModeMessages.push(function);
  m_syncExternalsWaiter.notify_all();
}

void C15Synth::queueExternalMidiOut(const dsp_host_dual::SimpleRawMidiMessage& m)
{
  m_externalMidiOutBuffer.push(m);
  m_syncExternalsWaiter.notify_all();
}

void C15Synth::onSplitPresetMessage(const nltools::msg::SplitPresetMessage& msg)
{
  const auto externalReset = m_dsp->onPresetMessage(msg);
  m_inputEventStage.requestExternalReset(externalReset);
}

void C15Synth::onSinglePresetMessage(const nltools::msg::SinglePresetMessage& msg)
{
  const auto externalReset = m_dsp->onPresetMessage(msg);
  m_inputEventStage.requestExternalReset(externalReset);
}

void C15Synth::onLayerPresetMessage(const nltools::msg::LayerPresetMessage& msg)
{
  const auto externalReset = m_dsp->onPresetMessage(msg);
  m_inputEventStage.requestExternalReset(externalReset);
}

void C15Synth::onNoteShiftMessage(const nltools::msg::Setting::NoteShiftMessage& msg)
{
  m_inputEventStage.setNoteShift(msg.m_shift);
}

void C15Synth::onPresetGlitchMessage(const nltools::msg::Setting::PresetGlitchMessage& msg)
{
  m_dsp->onSettingGlitchSuppr(msg.m_enabled);
}

void C15Synth::onTransitionTimeMessage(const nltools::msg::Setting::TransitionTimeMessage& msg)
{
  m_dsp->onSettingTransitionTime(msg.m_value);
}

void C15Synth::onEditSmoothingTimeMessage(const nltools::msg::Setting::EditSmoothingTimeMessage& msg)
{
  m_dsp->onSettingEditTime(msg.m_time);
}

void C15Synth::onTuneReferenceMessage(const nltools::msg::Setting::TuneReference& msg)
{
  m_dsp->onSettingTuneReference(static_cast<float>(msg.m_tuneReference));
}

void C15Synth::onMidiSettingsMessage(const nltools::msg::Setting::MidiSettingsMessage& msg)
{
  auto oldMsg = m_midiOptions.getLastReceivedMessage();
  m_midiOptions.update(msg);
  m_inputEventStage.onMidiSettingsMessageWasReceived(msg, oldMsg);
}

void C15Synth::onPanicNotificationReceived(const nltools::msg::PanicAudioEngine&)
{
  auto sendNotesOffOnChannel = [&](auto channel) {
    constexpr auto CCNum = static_cast<uint8_t>(MidiRuntimeOptions::MidiChannelModeMessageCCs::AllNotesOff);
    constexpr uint8_t CCModeChange = 0b10110000;
    const auto iChannel = MidiRuntimeOptions::channelEnumToInt(channel);

    if(iChannel != -1)
    {
      queueExternalMidiOut({ static_cast<uint8_t>(CCModeChange | iChannel), CCNum, 0 });
    }
  };

  resetDSP();

  sendNotesOffOnChannel(m_midiOptions.getMIDIPrimarySendChannel());
  if(m_dsp->getType() == SoundType::Split)
    sendNotesOffOnChannel(m_midiOptions.getMIDISplitSendChannel());
}

void C15Synth::onHWSourceSendMessageReceived(const nltools::msg::HWSourceSendChangedMessage& msg)
{
  m_inputEventStage.onSendParameterReceived(msg);
}
