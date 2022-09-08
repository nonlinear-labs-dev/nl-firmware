#pragma once

#include <nltools/messaging/Message.h>
#include <parameters/messaging/ParameterMessageFactory.h>
#include <groups/MonoGroup.h>
#include <tools/RecursionGuard.h>

namespace UNDO
{
  class Transaction;
}

class EditBuffer;
class Uuid;
class Settings;
class PresetManager;
class PlaycontrollerProxy;

class AudioEngineProxy : public sigc::trackable
{
 public:
  AudioEngineProxy(PresetManager& pm, Settings& settings, PlaycontrollerProxy& playProxy);

  template <typename tParameter> auto createAndSendParameterMessage(const tParameter* parameter)
  {
    if(!m_suppressParamChanges)
      sendParameterMessage(createMessage<tParameter>(parameter));
  }

  template <typename tParameter> auto createMessage(const tParameter* parameter)
  {
    return ParameterMessageFactory::createMessage<tParameter>(parameter);
  }

  template <class tMessage> void sendParameterMessage(const tMessage& msg)
  {
    if(!m_suppressParamChanges)
      send(nltools::msg::EndPoint::AudioEngine, msg);
  }

  template <typename tSettingMessage> void sendSettingMessage(const tSettingMessage& msg)
  {
    send(nltools::msg::EndPoint::AudioEngine, msg);
  }

  void sendEditBuffer();

  void freezeParameterMessages();
  void thawParameterMessages(bool send);

  static nltools::msg::LayerPresetMessage createLayerEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SplitPresetMessage createSplitEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SinglePresetMessage createSingleEditBufferMessage(const EditBuffer& eb);

 private:
  static void fillMonoPart(nltools::msg::ParameterGroups::MonoGroup& monoGroup, ParameterGroup* const& g);
  static void fillUnisonPart(nltools::msg::ParameterGroups::UnisonGroup& unisonGroup, ParameterGroup* const& g);

  void onMidiBankSelectionChanged(const Uuid& newMidiBankUuid);
  void setLastKnownMIDIProgramChangeNumber(int pc);
  void sendSelectedMidiPresetAsProgramChange();
  void onPresetManagerLoaded();

  uint8_t m_lastMIDIKnownProgramNumber = std::numeric_limits<uint8_t>::max();
  uint m_suppressParamChanges = 0;

  sigc::connection m_midiBankChangedConnection;
  sigc::connection m_midiBankConnection;
  RecursionGuard m_programChangeRecursion;
  template <typename T> void subscribeToMidiSetting(Settings* s);
  template <typename... TT> void subscribeToMidiSettings(Settings* s);
  void connectSettingsToAudioEngineMessage();
  void scheduleMidiSettingsMessage();

  Throttler m_sendMidiSettingThrottler;
  std::vector<sigc::connection> m_settingConnections;
  PresetManager& m_presetManager;
  Settings& m_settings;
  PlaycontrollerProxy& m_playcontrollerProxy;
};
