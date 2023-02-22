#include "AudioEngineProxy.h"
#include "parameters/PedalParameter.h"
#include "parameters/RibbonParameter.h"
#include "parameters/MacroControlSmoothingParameter.h"
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <groups/ScaleGroup.h>
#include <device-settings/Settings.h>
#include <parameter_declarations.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <device-settings/midi/MidiChannelSettings.h>
#include <device-settings/midi/receive/MidiReceiveAftertouchCurveSetting.h>
#include <device-settings/midi/receive/MidiReceiveVelocityCurveSetting.h>
#include <device-settings/midi/mappings/PedalCCMapping.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <device-settings/midi/mappings/AftertouchCCMapping.h>
#include <device-settings/midi/mappings/BenderCCMapping.h>
#include <presets/Preset.h>
#include <device-settings/midi/mappings/EnableHighVelocityCC.h>
#include <device-settings/midi/mappings/Enable14BitSupport.h>
#include <device-settings/flac/AutoStartRecorderSetting.h>
#include <device-settings/midi/RoutingSettings.h>
#include <use-cases/PhysicalControlParameterUseCases.h>
#include <use-cases/SettingsUseCases.h>
#include <use-cases/BankUseCases.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-settings/NoteShift.h>
#include <Application.h>
#include <PresetMessages.h>

AudioEngineProxy::AudioEngineProxy(PresetManager &pm, Settings &settings, PlaycontrollerProxy &playProxy)
    : m_sendMidiSettingThrottler(Application::get().getMainContext(), std::chrono::milliseconds { 250 })
    , m_presetManager { pm }
    , m_settings { settings }
    , m_playcontrollerProxy { playProxy }
{
  using namespace nltools::msg;
  onConnectionEstablished(EndPoint::AudioEngine, sigc::mem_fun(this, &AudioEngineProxy::sendEditBuffer));
  onConnectionEstablished(EndPoint::AudioEngine,
                          sigc::mem_fun(this, &AudioEngineProxy::connectSettingsToAudioEngineMessage));

  receive<HardwareSourcePollEnd>(EndPoint::Playground, [this](auto &msg) {
    int index = 0;
    bool didChange = false;
    for(auto value : msg.m_data)
    {
      auto param = findPhysicalControlParameterFromAudioEngineHWSourceID(index);
      index++;
      if(auto p = dynamic_cast<PhysicalControlParameter *>(param))
      {
        PhysicalControlParameterUseCases useCases(p);
        didChange |= useCases.applyPolledHWPosition(value);
      }
    }

    if(didChange)
    {
      nltools::Log::info("sending EditBuffer after PollEnd has been received!");
      sendEditBuffer();
    }
  });

  receive<HardwareSourceChangedNotification>(EndPoint::Playground, [this](auto &msg) {
    if(auto param = findPhysicalControlParameterFromAudioEngineHWSourceID(msg.hwSource))
    {
      if(auto p = dynamic_cast<PhysicalControlParameter *>(param))
      {
        PhysicalControlParameterUseCases useCase(p);
        useCase.changeFromAudioEngine(msg.position, msg.source);
        m_playcontrollerProxy.notifyRibbonTouch(p->getID().getNumber());
      }
    }
  });

  receive<Midi::ProgramChangeMessage>(EndPoint::Playground, [=](const auto &msg) {
    if(auto lock = m_programChangeRecursion.lock())
      if(auto bank = m_presetManager.findMidiSelectedBank())
      {
        setLastKnownMIDIProgramChangeNumber(static_cast<int>(msg.program));
        BankUseCases useCase(bank, m_settings);
        useCase.selectPreset(msg.program);
      }
  });

  receive<nltools::msg::Setting::SetGlobalLocalSetting>(EndPoint::Playground, [=](const auto &msg) {
    SettingsUseCases useCases(m_settings);
    useCases.setGlobalLocal(msg.m_state);
  });

  m_presetManager.onLoadHappened(sigc::mem_fun(this, &AudioEngineProxy::onPresetManagerLoaded));
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithMacroControlsAndMacroTimes(nltools::msg::detail::PresetMessage<tMsgType> &msg,
                                               const EditBuffer &editBuffer)
{
  size_t macroTimes = 0;
  size_t macroControls = 0;

  for(auto &param : editBuffer.getParameterGroupByID({ "MCs", VoiceGroup::Global })->getParameters())
  {
    const auto id = param->getID().getNumber();
    const auto index = C15::ParameterList[id].m_param.m_index;
    if(auto smoothing = dynamic_cast<const MacroControlSmoothingParameter *>(param))
    {
      auto &e = msg.m_macroTimes[index];
      e.m_id = static_cast<C15::PID::ParameterID>(id);
      e.m_controlPosition = smoothing->getControlPositionValue();
      macroTimes++;
    }
    else if(auto mc = dynamic_cast<const MacroControlParameter *>(param))
    {
      auto &e = msg.m_macroControls[index];
      e.m_id = static_cast<C15::PID::ParameterID>(id);
      e.m_controlPosition = mc->getControlPositionValue();
      macroControls++;
    }
  }

  nltools_assertAlways(msg.m_macroTimes.size() == macroTimes);
  nltools_assertAlways(msg.m_macroControls.size() == macroControls);
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithHardwareSources(nltools::msg::detail::PresetMessage<tMsgType> &msg, const EditBuffer &editBuffer)
{
  size_t hw = 0;

  for(auto &param : editBuffer.getParameterGroupByID({ "CS", VoiceGroup::Global })->getParameters())
  {
    if(auto hwParam = dynamic_cast<const PhysicalControlParameter *>(param))
    {
      const auto id = param->getID().getNumber();
      const auto index = C15::ParameterList[id].m_param.m_index;
      auto &e = msg.m_hardwareSources[index];
      e.m_id = static_cast<C15::PID::ParameterID>(id);
      e.m_controlPosition = hwParam->getControlPositionValue();
      e.m_returnMode = hwParam->getReturnMode();
      hw++;
    }
  }

  nltools_assertAlways(msg.m_hardwareSources.size() == hw);
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithHWAmounts(nltools::msg::detail::PresetMessage<tMsgType> &msg, const EditBuffer &editBuffer)
{
  size_t hwAmounts = 0;

  for(auto &param : editBuffer.getParameterGroupByID({ "MCM", VoiceGroup::Global })->getParameters())
  {
    const auto id = param->getID().getNumber();
    const auto index = C15::ParameterList[id].m_param.m_index;
    auto &e = msg.m_hardwareAmounts[index];
    e.m_id = static_cast<C15::PID::ParameterID>(id);
    e.m_controlPosition = param->getControlPositionValue();
    hwAmounts++;
  }

  nltools_assertAlways(hwAmounts == msg.m_hardwareAmounts.size());
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithGlobalNotHandledGroups(nltools::msg::detail::PresetMessage<tMsgType> &msg,
                                           const EditBuffer &editBuffer)
{
  const std::vector<std::string> alreadyParsedGroups = { "MCs", "CS", "MCM" };

  size_t modulateable = 0;
  size_t unmodulateable = 0;

  for(auto g : editBuffer.getParameterGroups(VoiceGroup::Global))
  {
    const auto wasNotAlreadyParsed
        = std::find(alreadyParsedGroups.begin(), alreadyParsedGroups.end(), g->getID().getName())
        == alreadyParsedGroups.end();

    if(wasNotAlreadyParsed)
    {
      for(auto &p : g->getParameters())
      {
        const auto id = p->getID().getNumber();
        const auto index = C15::ParameterList[id].m_param.m_index;
        if(auto modP = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &e = msg.m_globalModulateables[index];
          e.m_id = static_cast<C15::PID::ParameterID>(id);
          e.m_controlPosition = modP->getControlPositionValue();
          e.m_modulationAmount = modP->getModulationAmount();
          e.m_macro = modP->getModulationSource();
          modulateable++;
        }
        else
        {
          auto &e = msg.m_globalUnmodulateables[index];
          e.m_id = static_cast<C15::PID::ParameterID>(id);
          e.m_controlPosition = p->getControlPositionValue();
          unmodulateable++;
        }
      }
    }
  }

  nltools_assertAlways(msg.m_globalModulateables.size() == modulateable);
  nltools_assertAlways(msg.m_globalUnmodulateables.size() == unmodulateable);
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithMonophonicParameters(nltools::msg::detail::PresetMessage<tMsgType> &msg,
                                         const EditBuffer &editBuffer)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    size_t mono_modulateable = 0;
    size_t mono_unmodulateable = 0;

    auto &mono_modulateables = msg.m_monophonicModulateables[static_cast<int>(vg)];
    auto &mono_unmodulateables = msg.m_monophonicUnmodulateables[static_cast<int>(vg)];

    for(auto g : editBuffer.getParameterGroups(vg))
    {
      for(auto p : g->getParameters())
      {
        const auto id = p->getID().getNumber();
        const auto index = C15::ParameterList[id].m_param.m_index;
        switch(p->getType())
        {
          case C15::Descriptors::ParameterType::Monophonic_Modulateable:
          {
            if(auto modP = dynamic_cast<ModulateableParameter *>(p))
            {
              auto &e = mono_modulateables[index];
              e.m_id = static_cast<C15::PID::ParameterID>(id);
              e.m_controlPosition = modP->getControlPositionValue();
              e.m_modulationAmount = modP->getModulationAmount();
              e.m_macro = modP->getModulationSource();
              mono_modulateable++;
            }
            break;
          }
          case C15::Descriptors::ParameterType::Monophonic_Unmodulateable:
          {
            auto &e = mono_unmodulateables[index];
            e.m_id = static_cast<C15::PID::ParameterID>(id);
            e.m_controlPosition = p->getControlPositionValue();
            mono_unmodulateable++;
            break;
          }
          default:
            break;
        }
      }
    }

    nltools_assertAlways(mono_unmodulateable == mono_unmodulateables.size());
    nltools_assertAlways(mono_modulateable == mono_modulateables.size());
  }
}

template <nltools::msg::MessageType tMsgType>
void fillMessageWithSharedParameters(nltools::msg::detail::PresetMessage<tMsgType> &msg, const EditBuffer &editBuffer)
{
  fillMessageWithMacroControlsAndMacroTimes(msg, editBuffer);
  fillMessageWithHardwareSources(msg, editBuffer);
  fillMessageWithHWAmounts(msg, editBuffer);
  fillMessageWithGlobalNotHandledGroups(msg, editBuffer);
  fillMessageWithMonophonicParameters(msg, editBuffer);
}

void fillSingleMessageWithPolyParameters(nltools::msg::SinglePresetMessage &msg, const EditBuffer &eb)
{
  size_t modulateables = 0;
  size_t unmodulateables = 0;

  for(auto &g : eb.getParameterGroups(VoiceGroup::I))
  {
    for(auto &p : g->getParameters())
    {
      const auto id = p->getID().getNumber();
      const auto index = C15::ParameterList[id].m_param.m_index;
      switch(p->getType())
      {
        case C15::Descriptors::ParameterType::Polyphonic_Modulateable:
        {
          if(auto modP = dynamic_cast<ModulateableParameter *>(p))
          {
            auto &e = msg.m_polyphonicModulateables[index];
            e.m_id = static_cast<C15::PID::ParameterID>(id);
            e.m_controlPosition = modP->getControlPositionValue();
            e.m_modulationAmount = modP->getModulationAmount();
            e.m_macro = modP->getModulationSource();
            modulateables++;
          }
          break;
        }
        case C15::Descriptors::ParameterType::Polyphonic_Unmodulateable:
        {
          auto &e = msg.m_polyphonicUnmodulateables[index];
          e.m_id = static_cast<C15::PID::ParameterID>(id);
          e.m_controlPosition = p->getControlPositionValue();
          unmodulateables++;
          break;
        }
        default:
          break;
      }
    }
  }

  nltools_assertAlways(unmodulateables == msg.m_polyphonicUnmodulateables.size());
  nltools_assertAlways(modulateables == msg.m_polyphonicModulateables.size());
}

template <typename tMSG> void fillDualMessageWithPolyParameters(tMSG &msg, const EditBuffer &editBuffer)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    auto &modParams = msg.m_polyphonicModulateables[static_cast<int>(vg)];
    auto &unmodParams = msg.m_polyphonicUnmodulateables[static_cast<int>(vg)];

    size_t modulateables = 0;
    size_t unmodulateables = 0;

    for(auto &g : editBuffer.getParameterGroups(vg))
    {
      for(auto &p : g->getParameters())
      {
        const auto id = p->getID().getNumber();
        const auto index = C15::ParameterList[id].m_param.m_index;
        switch(p->getType())
        {
          case C15::Descriptors::ParameterType::Polyphonic_Modulateable:
          {
            if(auto modP = dynamic_cast<ModulateableParameter *>(p))
            {
              auto &e = modParams[index];
              e.m_id = static_cast<C15::PID::ParameterID>(id);
              e.m_controlPosition = modP->getControlPositionValue();
              e.m_modulationAmount = modP->getModulationAmount();
              e.m_macro = modP->getModulationSource();
              modulateables++;
            }
            break;
          }
          case C15::Descriptors::ParameterType::Polyphonic_Unmodulateable:
          {
            auto &e = unmodParams[index];
            e.m_id = static_cast<C15::PID::ParameterID>(id);
            e.m_controlPosition = p->getControlPositionValue();
            unmodulateables++;
            break;
          }
          default:
            break;
        }
      }
    }

    nltools_assertAlways(unmodulateables == unmodParams.size());
    nltools_assertAlways(modulateables == modParams.size());
  }
}

nltools::msg::SinglePresetMessage AudioEngineProxy::createSingleEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::SinglePresetMessage msg {};
  fillMessageWithSharedParameters(msg, eb);
  fillSingleMessageWithPolyParameters(msg, eb);
  nltools_assertAlways(nltools::msg::SinglePresetMessage::validate(msg));
  return msg;
}

nltools::msg::SplitPresetMessage AudioEngineProxy::createSplitEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::SplitPresetMessage msg {};
  fillMessageWithSharedParameters(msg, eb);
  fillDualMessageWithPolyParameters(msg, eb);
  nltools_assertAlways(nltools::msg::SplitPresetMessage::validate(msg));
  return msg;
}

nltools::msg::LayerPresetMessage AudioEngineProxy::createLayerEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::LayerPresetMessage msg {};
  fillMessageWithSharedParameters(msg, eb);
  fillDualMessageWithPolyParameters(msg, eb);
  // identical Unison Detune/Phase/Pan and Mono Glide are required (not sure if EditBuffer already facilitates this)
  msg.guaranteeEqualVoicesParameters();
  nltools_assertAlways(nltools::msg::LayerPresetMessage::validate(msg));
  return msg;
}

void AudioEngineProxy::sendEditBuffer()
{
  auto eb = m_presetManager.getEditBuffer();

  switch(eb->getType())
  {
    case SoundType::Single:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createSingleEditBufferMessage(*eb));
      break;
    case SoundType::Split:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createSplitEditBufferMessage(*eb));
      break;
    case SoundType::Layer:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createLayerEditBufferMessage(*eb));
      break;
    case SoundType::Invalid:
      break;
  }
}

void AudioEngineProxy::freezeParameterMessages()
{
  m_suppressParamChanges++;
}

void AudioEngineProxy::thawParameterMessages(bool send)
{
  m_suppressParamChanges--;

  if(m_suppressParamChanges == 0 && send)
    sendEditBuffer();
}

void AudioEngineProxy::onMidiBankSelectionChanged(const Uuid &newMidiBankUUID)
{
  m_midiBankChangedConnection.disconnect();

  if(auto midiBank = m_presetManager.findBank(newMidiBankUUID))
  {
    m_midiBankChangedConnection
        = midiBank->onBankChanged(sigc::mem_fun(this, &AudioEngineProxy::sendSelectedMidiPresetAsProgramChange));
  }
}

void AudioEngineProxy::sendSelectedMidiPresetAsProgramChange()
{
  if(auto midiBank = m_presetManager.findMidiSelectedBank())
  {
    if(auto selectedPreset = midiBank->getSelectedPreset())
    {
      uint8_t presetPos = midiBank->getPresetPosition(selectedPreset);
      if(m_lastMIDIKnownProgramNumber != presetPos)
      {
        if(presetPos < 128)
        {
          m_lastMIDIKnownProgramNumber = presetPos;
          nltools::msg::Midi::ProgramChangeMessage msg {};
          msg.program = presetPos;
          selectedPreset->getType();
          nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
        }
      }
    }
  }
}

void AudioEngineProxy::onPresetManagerLoaded()
{
  m_midiBankConnection
      = m_presetManager.onMidiBankSelectionHappened(sigc::mem_fun(this, &AudioEngineProxy::onMidiBankSelectionChanged));
}

template <typename T> void AudioEngineProxy::subscribeToMidiSetting(Settings *s)
{
  if(auto setting = s->getSetting<T>())
  {
    m_settingConnections.push_back(
        setting->onChange(sigc::hide(sigc::mem_fun(this, &AudioEngineProxy::scheduleMidiSettingsMessage))));
  }
}

template <typename... TT> void AudioEngineProxy::subscribeToMidiSettings(Settings *s)
{
  (subscribeToMidiSetting<TT>(s), ...);
}

void AudioEngineProxy::connectSettingsToAudioEngineMessage()
{
  m_settingConnections.clear();

  subscribeToMidiSettings<MidiReceiveChannelSetting, MidiReceiveChannelSplitSetting, MidiReceiveAftertouchCurveSetting,
                          MidiReceiveVelocityCurveSetting, MidiSendChannelSetting, MidiSendChannelSplitSetting,
                          PedalCCMapping<1>, PedalCCMapping<2>, PedalCCMapping<3>, PedalCCMapping<4>,
                          RibbonCCMapping<1>, RibbonCCMapping<2>, RibbonCCMapping<1>, RibbonCCMapping<2>,
                          AftertouchCCMapping, BenderCCMapping, EnableHighVelocityCC, Enable14BitSupport,
                          RoutingSettings, GlobalLocalEnableSetting, SelectedRibbonsSetting>(&m_settings);

  m_settingConnections.push_back(m_settings.getSetting<AutoStartRecorderSetting>()->onChange([](const Setting *s) {
    auto as = static_cast<const AutoStartRecorderSetting *>(s);
    const auto shouldAutoStart = as->get();
    auto msg = nltools::msg::Setting::FlacRecorderAutoStart {};
    msg.enabled = shouldAutoStart;
    nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
  }));
}

void AudioEngineProxy::scheduleMidiSettingsMessage()
{
  m_sendMidiSettingThrottler.doTask([this]() {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.sendChannel = m_settings.getSetting<MidiSendChannelSetting>()->get();
    msg.sendSplitChannel = m_settings.getSetting<MidiSendChannelSplitSetting>()->get();
    msg.receiveChannel = m_settings.getSetting<MidiReceiveChannelSetting>()->get();
    msg.receiveSplitChannel = m_settings.getSetting<MidiReceiveChannelSplitSetting>()->get();

    msg.pedal1cc = m_settings.getSetting<PedalCCMapping<1>>()->get();
    msg.pedal2cc = m_settings.getSetting<PedalCCMapping<2>>()->get();
    msg.pedal3cc = m_settings.getSetting<PedalCCMapping<3>>()->get();
    msg.pedal4cc = m_settings.getSetting<PedalCCMapping<4>>()->get();
    msg.ribbon1cc = m_settings.getSetting<RibbonCCMapping<1>>()->get();
    msg.ribbon2cc = m_settings.getSetting<RibbonCCMapping<2>>()->get();
    msg.ribbon3cc = m_settings.getSetting<RibbonCCMapping<3>>()->get();
    msg.ribbon4cc = m_settings.getSetting<RibbonCCMapping<4>>()->get();
    msg.aftertouchcc = m_settings.getSetting<AftertouchCCMapping>()->get();
    msg.bendercc = m_settings.getSetting<BenderCCMapping>()->get();

    msg.highVeloCCEnabled = m_settings.getSetting<EnableHighVelocityCC>()->get();
    msg.highResCCEnabled = m_settings.getSetting<Enable14BitSupport>()->get();

    msg.routings = m_settings.getSetting<RoutingSettings>()->getRaw();
    msg.localEnable = m_settings.getSetting<GlobalLocalEnableSetting>()->get();

    msg.localEnable = m_settings.getSetting<GlobalLocalEnableSetting>()->get();

    msg.isSecondSetOfRibbonsEnabled
        = m_settings.getSetting<SelectedRibbonsSetting>()->get() == SelectedRibbons::Ribbon3_4;

    nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
  });
}

void AudioEngineProxy::setLastKnownMIDIProgramChangeNumber(int pc)
{
  m_lastMIDIKnownProgramNumber = pc;
}

Parameter *AudioEngineProxy::findPhysicalControlParameterFromAudioEngineHWSourceID(int index)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto i = [](auto s) { return static_cast<int>(s); };

  switch(index)
  {
    case i(HardwareSource_AE::PEDAL1):
      return eb->findParameterByID({ C15::PID::Pedal_1, VoiceGroup::Global });
    case i(HardwareSource_AE::PEDAL2):
      return eb->findParameterByID({ C15::PID::Pedal_2, VoiceGroup::Global });
    case i(HardwareSource_AE::PEDAL3):
      return eb->findParameterByID({ C15::PID::Pedal_3, VoiceGroup::Global });
    case i(HardwareSource_AE::PEDAL4):
      return eb->findParameterByID({ C15::PID::Pedal_4, VoiceGroup::Global });
    case i(HardwareSource_AE::RIBBON1):
      return eb->findParameterByID({ C15::PID::Ribbon_1, VoiceGroup::Global });
    case i(HardwareSource_AE::RIBBON2):
      return eb->findParameterByID({ C15::PID::Ribbon_2, VoiceGroup::Global });
    case i(HardwareSource_AE::RIBBON3):
      return eb->findParameterByID({ C15::PID::Ribbon_3, VoiceGroup::Global });
    case i(HardwareSource_AE::RIBBON4):
      return eb->findParameterByID({ C15::PID::Ribbon_4, VoiceGroup::Global });
    case i(HardwareSource_AE::BENDER):
      return eb->findParameterByID({ C15::PID::Bender, VoiceGroup::Global });
    case i(HardwareSource_AE::AFTERTOUCH):
      return eb->findParameterByID({ C15::PID::Aftertouch, VoiceGroup::Global });
    default:
      return nullptr;
  }
}

void AudioEngineProxy::freezePresetMessages()
{
  m_suppressPresetChanges++;
}

void AudioEngineProxy::thawPresetMessages(bool send)
{
  m_suppressPresetChanges--;

  if(m_suppressPresetChanges == 0 && send)
    sendEditBuffer();
}
