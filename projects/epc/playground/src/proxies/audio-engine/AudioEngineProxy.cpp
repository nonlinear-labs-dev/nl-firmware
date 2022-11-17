#include "AudioEngineProxy.h"
#include "parameters/PedalParameter.h"
#include "parameters/RibbonParameter.h"
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <groups/MacroControlsGroup.h>
#include <groups/MonoGroup.h>
#include <groups/UnisonGroup.h>
#include <groups/ScaleGroup.h>
#include <device-settings/Settings.h>
#include <parameter_declarations.h>
#include <groups/SplitParameterGroups.h>
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

  receive<HardwareSourcePollEnd>(EndPoint::Playground,
                                 [this](auto &msg)
                                 {
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

  receive<HardwareSourceChangedNotification>(EndPoint::Playground,
                                             [this](auto &msg)
                                             {
                                               if(auto param
                                                  = findPhysicalControlParameterFromAudioEngineHWSourceID(msg.hwSource))
                                               {
                                                 if(auto p = dynamic_cast<PhysicalControlParameter *>(param))
                                                 {
                                                   PhysicalControlParameterUseCases useCase(p);
                                                   useCase.changeFromAudioEngine(msg.position, msg.source);
                                                   m_playcontrollerProxy.notifyRibbonTouch(p->getID().getNumber());
                                                 }
                                               }
                                             });

  receive<Midi::ProgramChangeMessage>(EndPoint::Playground,
                                      [=](const auto &msg)
                                      {
                                        if(auto lock = m_programChangeRecursion.lock())
                                          if(auto bank = m_presetManager.findMidiSelectedBank())
                                          {
                                            setLastKnownMIDIProgramChangeNumber(static_cast<int>(msg.program));
                                            BankUseCases useCase(bank, m_settings);
                                            useCase.selectPreset(msg.program);
                                          }
                                      });

  receive<nltools::msg::Setting::SetGlobalLocalSetting>(EndPoint::Playground,
                                                        [=](const auto &msg)
                                                        {
                                                          SettingsUseCases useCases(m_settings);
                                                          useCases.setGlobalLocal(msg.m_state);
                                                        });

  m_presetManager.onLoadHappened(sigc::mem_fun(this, &AudioEngineProxy::onPresetManagerLoaded));
}

template <typename tMsg> void fillMessageWithGlobalParams(tMsg &msg, const EditBuffer &editBuffer)
{
  size_t hwSource = 0;
  size_t scaleOffsets = 0;
  size_t mc = 0;
  size_t mcT = 0;
  size_t modR = 0;

  auto masterParameter
      = editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Volume, VoiceGroup::Global });
  nltools_assertOnDevPC(masterParameter != nullptr);
  auto &master = msg.master.volume;
  master.id = masterParameter->getID().getNumber();
  master.controlPosition = masterParameter->getControlPositionValue();
  master.modulationAmount = masterParameter->getModulationAmount();
  master.mc = masterParameter->getModulationSource();

  auto tuneParameter
      = editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Tune, VoiceGroup::Global });
  nltools_assertOnDevPC(tuneParameter != nullptr);
  auto &tune = msg.master.tune;
  tune.id = tuneParameter->getID().getNumber();
  tune.controlPosition = tuneParameter->getControlPositionValue();
  tune.modulationAmount = tuneParameter->getModulationAmount();
  tune.mc = tuneParameter->getModulationSource();

  auto panParameter
      = editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Pan, VoiceGroup::Global });
  nltools_assertOnDevPC(panParameter != nullptr);
  auto &pan = msg.master.pan;
  pan.id = panParameter->getID().getNumber();
  pan.controlPosition = panParameter->getControlPositionValue();
  pan.modulationAmount = panParameter->getModulationAmount();
  pan.mc = panParameter->getModulationSource();

  auto sepParam
      = editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Serial_FX, VoiceGroup::Global });
  nltools_assertOnDevPC(sepParam != nullptr);
  auto &sep = msg.master.serialFX;
  sep.id = sepParam->getID().getNumber();
  sep.controlPosition = sepParam->getControlPositionValue();
  sep.modulationAmount = sepParam->getModulationAmount();
  sep.mc = sepParam->getModulationSource();

  for(auto &g : editBuffer.getParameterGroups(VoiceGroup::Global))
  {
    for(auto p : g->getParameters())
    {
      auto isScale = dynamic_cast<ScaleGroup *>(p->getParentGroup()) != nullptr;

      if(auto hwSrcParam = dynamic_cast<PhysicalControlParameter *>(p))
      {
        auto &pItem = msg.hwsources[hwSource++];
        pItem.id = p->getID().getNumber();
        pItem.controlPosition = p->getControlPositionValue();
        pItem.returnMode = hwSrcParam->getReturnMode();
      }
      else if(isScale)
      {
        if(p->getID().getNumber() == C15::PID::Scale_Base_Key)
        {
          nltools_assertOnDevPC(dynamic_cast<ModulateableParameter *>(p) == nullptr);
          auto &pItem = msg.scaleBaseKey;
          pItem.id = p->getID().getNumber();
          pItem.controlPosition = p->getControlPositionValue();
        }
        else if(auto modP = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &pItem = msg.scaleOffsets[scaleOffsets++];
          pItem.id = p->getID().getNumber();
          pItem.controlPosition = p->getControlPositionValue();
          pItem.mc = modP->getModulationSource();
          pItem.modulationAmount = modP->getModulationAmount();
        }
      }
      else if(auto mcParameter = dynamic_cast<MacroControlParameter *>(p))
      {
        auto &macro = msg.macros[mc++];
        macro.id = mcParameter->getID().getNumber();
        macro.controlPosition = mcParameter->getControlPositionValue();
      }
      else if(auto hwAmounts = dynamic_cast<ModulationRoutingParameter *>(p))
      {
        auto &hwAmount = msg.hwamounts[modR++];
        hwAmount.id = hwAmounts->getID().getNumber();
        hwAmount.controlPosition = hwAmounts->getControlPositionValue();
      }
      else if(MacroControlsGroup::isMacroTime(p->getID()))
      {
        auto &mcTime = msg.macrotimes[mcT++];
        mcTime.id = p->getID().getNumber();
        mcTime.controlPosition = p->getControlPositionValue();
      }
    }
  }

  nltools_assertAlways(msg.scaleOffsets.size() == scaleOffsets);
  nltools_assertAlways(msg.hwsources.size() == hwSource);
  nltools_assertAlways(msg.macros.size() == mc);
  nltools_assertAlways(msg.hwamounts.size() == modR);
  nltools_assertAlways(msg.macrotimes.size() == mcT);
}

template <typename tParameterType, typename tParameterArray>
void forEachParameterInGroup(const EditBuffer *eb, const GroupId &group, tParameterArray &array, size_t &index)
{
  for(auto &p : eb->getParameterGroupByID(group)->getParameters())
  {
    if(auto param = dynamic_cast<tParameterType *>(p))
    {
      auto &msgParam = array[index++];
      msgParam.controlPosition = param->getControlPositionValue();
      msgParam.id = param->getID().getNumber();
    }
  }
}

nltools::msg::SinglePresetMessage AudioEngineProxy::createSingleEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::SinglePresetMessage msg {};
  fillMessageWithGlobalParams(msg, eb);

  size_t mc = 0;
  size_t modR = 0;
  size_t modP = 0;
  size_t unMod = 0;

  forEachParameterInGroup<MacroControlParameter>(&eb, { "MCs", VoiceGroup::Global }, msg.macros, mc);
  forEachParameterInGroup<ModulationRoutingParameter>(&eb, { "MCM", VoiceGroup::Global }, msg.hwamounts, modR);

  for(auto &g : eb.getParameterGroups(VoiceGroup::I))
  {
    if(auto unisonGroup = dynamic_cast<UnisonGroup *>(g))
    {
      fillUnisonPart(msg.unison, unisonGroup);
    }
    else if(auto monoGroup = dynamic_cast<MonoGroup *>(g))
    {
      fillMonoPart(msg.mono, monoGroup);
    }
    else if(auto splitGroup = dynamic_cast<SplitParameterGroups *>(g))
    {
      //Ignore
    }
    else
    {
      for(auto p : g->getParameters())
      {
        if(auto modParam = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &mod = msg.modulateables[modP++];
          mod.id = modParam->getID().getNumber();
          mod.controlPosition = modParam->getControlPositionValue();
          mod.modulationAmount = modParam->getModulationAmount();
          mod.mc = modParam->getModulationSource();
        }
        else
        {
          auto &unModulateable = msg.unmodulateables[unMod++];
          unModulateable.id = p->getID().getNumber();
          unModulateable.controlPosition = p->getControlPositionValue();
        }
      }
    }
  }

  nltools_assertAlways(msg.modulateables.size() == modP);
  nltools_assertAlways(msg.unmodulateables.size() == unMod);
  nltools_assertAlways(msg.macros.size() == mc);
  nltools_assertAlways(msg.hwamounts.size() == modR);
  return msg;
}

void AudioEngineProxy::fillMonoPart(nltools::msg::ParameterGroups::MonoGroup &monoGroup, ParameterGroup *const &g)
{
  auto from = g->getVoiceGroup();

  if(auto enable = g->findParameterByID({ C15::PID::Mono_Grp_Enable, from }))
  {
    nltools_assertOnDevPC(dynamic_cast<ModulateableParameter *>(enable) == nullptr);
    auto &monoEnable = monoGroup.monoEnable;
    monoEnable.id = C15::PID::Mono_Grp_Enable;
    monoEnable.controlPosition = enable->getControlPositionValue();
  }

  if(auto prio = g->findParameterByID({ C15::PID::Mono_Grp_Prio, from }))
  {
    nltools_assertOnDevPC(dynamic_cast<ModulateableParameter *>(prio) == nullptr);
    auto &item = monoGroup.priority;
    item.id = C15::PID::Mono_Grp_Prio;
    item.controlPosition = prio->getControlPositionValue();
  }

  if(auto legato = g->findParameterByID({ C15::PID::Mono_Grp_Legato, from }))
  {
    nltools_assertOnDevPC(dynamic_cast<ModulateableParameter *>(legato) == nullptr);
    auto &item = monoGroup.legato;
    item.id = C15::PID::Mono_Grp_Legato;
    item.controlPosition = legato->getControlPositionValue();
  }

  if(auto glide = dynamic_cast<ModulateableParameter *>(g->findParameterByID({ C15::PID::Mono_Grp_Glide, from })))
  {
    auto &item = monoGroup.glide;
    item.id = C15::PID::Mono_Grp_Glide;
    item.controlPosition = glide->getControlPositionValue();
    item.mc = glide->getModulationSource();
    item.modulationAmount = glide->getModulationAmount();
  }
}

void AudioEngineProxy::fillUnisonPart(nltools::msg::ParameterGroups::UnisonGroup &unisonGroup, ParameterGroup *const &g)
{
  auto from = g->getVoiceGroup();

  if(auto unisonParam = g->getParameterByID({ C15::PID::Unison_Voices, from }))
  {
    auto &unisonVoices = unisonGroup.unisonVoices;
    unisonVoices.id = C15::PID::Unison_Voices;
    unisonVoices.controlPosition = unisonParam->getControlPositionValue();
  }

  if(auto unisonDetune = dynamic_cast<ModulateableParameter *>(g->getParameterByID({ C15::PID::Unison_Detune, from })))
  {
    auto &detune = unisonGroup.detune;
    detune.id = unisonDetune->getID().getNumber();
    detune.controlPosition = unisonDetune->getControlPositionValue();
    detune.mc = unisonDetune->getModulationSource();
    detune.modulationAmount = unisonDetune->getModulationAmount();
  }

  if(auto unisonPan = g->getParameterByID(
         { C15::PID::Unison_Pan, from }))  // previously was 252: Unison Phase (seems like a bugfix..?)
  {
    auto &pan = unisonGroup.pan;
    pan.id = unisonPan->getID().getNumber();
    pan.controlPosition = unisonPan->getControlPositionValue();
  }

  if(auto unisonPhase = g->getParameterByID(
         { C15::PID::Unison_Phase, from }))  // previously was 253: Unison Pan (seems like a bugfix..?)
  {
    auto &phase = unisonGroup.phase;
    phase.id = unisonPhase->getID().getNumber();
    phase.controlPosition = unisonPhase->getControlPositionValue();
  }
}

template <typename tMsg> void fillDualMessage(tMsg &msg, const EditBuffer &editBuffer)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    size_t modP = 0;
    size_t unMod = 0;

    auto arrayIndex = static_cast<int>(vg);
    for(auto &g : editBuffer.getParameterGroups(vg))
    {
      if(dynamic_cast<UnisonGroup *>(g) || dynamic_cast<MonoGroup *>(g))
        continue;

      for(auto p : g->getParameters())
      {
        if(auto modParam = dynamic_cast<ModulateableParameter *>(p))
        {
          if(modParam->getID().getNumber() != C15::PID::Split_Split_Point)
          {
            auto &mod = msg.modulateables[arrayIndex][modP++];
            mod.id = modParam->getID().getNumber();
            mod.controlPosition = modParam->getControlPositionValue();
            mod.modulationAmount = modParam->getModulationAmount();
            mod.mc = modParam->getModulationSource();
          }
        }
        else
        {
          if(p->getID().getNumber() != C15::PID::Unison_Voices && p->getID().getNumber() != C15::PID::Mono_Grp_Enable)
          {
            auto &unModulateable = msg.unmodulateables[arrayIndex][unMod++];
            unModulateable.id = p->getID().getNumber();
            unModulateable.controlPosition = p->getControlPositionValue();
          }
        }
      }
    }

    nltools_assertAlways(msg.modulateables[arrayIndex].size() == modP);
    nltools_assertAlways(msg.unmodulateables[arrayIndex].size() == unMod);
  }
}

nltools::msg::SplitPresetMessage AudioEngineProxy::createSplitEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::SplitPresetMessage msg {};
  fillMessageWithGlobalParams(msg, eb);
  fillDualMessage(msg, eb);

  for(auto i = 0; i < 2; i++)
  {
    const auto vg = i == 0 ? VoiceGroup::I : VoiceGroup::II;
    auto param = dynamic_cast<ModulateableParameter *>(eb.findParameterByID({ C15::PID::Split_Split_Point, vg }));

    auto &t = msg.splitpoint[i];
    t.id = param->getID().getNumber();
    t.controlPosition = param->getControlPositionValue();
    t.modulationAmount = param->getModulationAmount();
    t.mc = param->getModulationSource();
  }

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    auto vgIndex = static_cast<int>(vg);

    if(auto monoGroup = eb.getParameterGroupByID({ "Mono", vg }))
    {
      fillMonoPart(msg.mono[vgIndex], monoGroup);
    }

    if(auto unisonGroup = eb.getParameterGroupByID({ "Unison", vg }))
    {
      fillUnisonPart(msg.unison[vgIndex], unisonGroup);
    }
  }

  return msg;
}

nltools::msg::LayerPresetMessage AudioEngineProxy::createLayerEditBufferMessage(const EditBuffer &eb)
{
  nltools::msg::LayerPresetMessage msg {};
  fillMessageWithGlobalParams(msg, eb);
  fillDualMessage(msg, eb);

  if(auto unisonGroup = eb.getParameterGroupByID({ "Unison", VoiceGroup::I }))
    fillUnisonPart(msg.unison, unisonGroup);

  if(auto monoGroup = eb.getParameterGroupByID({ "Mono", VoiceGroup::I }))
    fillMonoPart(msg.mono, monoGroup);

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
          msg.programType = selectedPreset->getType();
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

  m_settingConnections.push_back(m_settings.getSetting<AutoStartRecorderSetting>()->onChange(
      [](const Setting *s)
      {
        auto as = static_cast<const AutoStartRecorderSetting *>(s);
        const auto shouldAutoStart = as->get();
        auto msg = nltools::msg::Setting::FlacRecorderAutoStart {};
        msg.enabled = shouldAutoStart;
        nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
      }));
}

void AudioEngineProxy::scheduleMidiSettingsMessage()
{
  m_sendMidiSettingThrottler.doTask(
      [this]()
      {
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
